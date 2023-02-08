#pragma once
#include <netinet/in.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include "exeception.h"
// typedef struct sv_server{
//     int sock_f;
//     struct sockaddr_in address;
//     int port;
//     int opt;
//     int maxconn;
//     int sizesockets;
//     struct pollfd* sockets;
//     sv_user_packs_t* users;
//     sv_arraypack_t arrpack;
//     sv_arrayd_t userpacks;

//     int nowconnect;
// }sv_server_t;
namespace server {

class user {
 protected:
 public:
  user() {}
};
template <class T>
class array {
 private:
  char* arrptr;
  int elementsize;
  int capacitynumber;
  int number;
  int tsize;

 public:
  array();
  array(int number);
  array(int number, int elementsize);
  void setelementsize(int elmsize);
  void resize(int n);
  int size();
  T& operator[](int index);
};

struct pack {
  char headpack[8];
  int realsize = 0;
  char* data = NULL;
  int allsize = 0;
};
struct serveruser {
  int idpack=0;
  pack lastpack;
  void Clear() {
    pack n;
    lastpack = n;
  }
};
struct usersock{
  int sock;
  int iduser;
};
class Users {
 private:
  std::vector<pollfd> userssockets;
  array<user> clientusers;
  std::vector<serveruser> serverusers;
  int localindexpoll = 0;

 public:
  void initusers(int sock, int maxusers, user* user, int sizeuser) {
    clientusers.setelementsize(sizeuser);
    clientusers.resize(maxusers);
    serverusers.resize(maxusers);
    userssockets.resize(maxusers);
    localindexpoll=maxusers-1;
    for (int i = 0; i < maxusers; i++) {
      memcpy(&clientusers[i], user, sizeuser);
      userssockets[i].fd = -1;
      serverusers[i].idpack=i;
    }
    userssockets[0].fd = sock;
    userssockets[0].events = POLLIN;
    userssockets[0].revents = 0;
  }
  void regusersocket(usersock user){

  }
  usersock geteventusers(bool* newconn) {
    if (localindexpoll == userssockets.size() - 1) {
      int ret = poll(&userssockets[0], userssockets.size(), -1);
      if (ret < 0) {
        throw NetworkExeption("ERROR POLL");
      }
      localindexpoll=0;
    }
    for (int i = localindexpoll; i < userssockets.size(); i++) {
      if (userssockets[i].revents && POLLIN) {
        userssockets[i].revents &= ~POLLIN;
        if(i==0){
          *newconn=true;
        }else{
          *newconn=false;
        }
        usersock s;
        s.iduser=i;
        s.sock=userssockets[i].fd;
        localindexpoll++;
        return s;
      }
    }
    throw NetworkExeption("NONE");
  }
  bool emptylastpack(int iduser) {
    if (serverusers[iduser].lastpack.data == NULL)
      return true;

    return false;
  }
  void addlastpack(int iduser, pack pk) { serverusers[iduser].lastpack = pk; }
  void disconnect(int iduser) {
    // clear
    serverusers[iduser].Clear();
  }
  void adddata(int iduser, int addsize) {
    serverusers[iduser].lastpack.realsize += addsize;
  }
};
class NetworkEvent {
 private:
  Users users;
  std::vector<pack> packs;
  int sock;
  struct sockaddr_in address;
  int port;

 private:
  void getevent();
  void processpack();
  void newpack();
  void addlastpack();
 public:
  void initsock(int sock, int maxclient, user* user, int sizeuser);
  void start();
};
class serv {
 private:
  int port;
  int maxconn;
  NetworkEvent net;
  int nowconnect;

 public:
  serv();
  serv(int port, int maxconn);
  void initserver(int port, int maxconn);
  void start_server(user* user, int sizeuser);
};
}  // namespace server
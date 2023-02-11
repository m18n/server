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
 public:
  int iduser = 0;
  pack lastpack;

 public:
  void Clear() {
    pack n;
    lastpack = n;
  }
};
struct usersock {
  int sock;
  int iduser;
};
class Users {
 private:
  std::vector<pollfd> userssockets;
  array<user> clientusers;
  std::vector<serveruser> serverusers;
  int uparrays = 0;

 public:
  void initusers(int sock, int maxusers, user* user, int sizeuser);
  void regusersocket(int sock);
  void geteventusers(std::vector<usersock>* retarray);
  bool emptylastpack(int iduser);
  void reglastpack(int iduser, pack pk);
  void disconnect(int iduser);
  void adddata(int iduser, int addsize);
};
class serv {
 private:
  int port;
  int maxconn;
  int sock;
  struct sockaddr_in address;
  int nowconnect;
  Users users;
  std::vector<pack> packs;
  bool init;

 private:
  void getevent();
  void processpack();
  void initsocket(user* user, int sizeuser);

 public:
  serv();
  serv(int port, int maxconn, user* user, int sizeuser);
  void initserver(int port, int maxconn, user* user, int sizeuser);
  void start_server();
};
}  // namespace server
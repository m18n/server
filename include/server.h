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
#include <vector>
#include "exeception.h"
#include<thread>
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
  pollfd* pollptr;

 public:
  user() { pollptr = NULL; }
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

struct clients{
  std::vector<pollfd> sockets;
  array<user> users;
  void initclients(int maxclients, user* user, int sizeuser);
};
class serv {
 private:
  int sock;
  struct sockaddr_in address;
  int port;
  int maxconn;
  clients cls;
  int nowconnect;

 private:
  // void getpack();
  // void processpack();
  void getevent();
  void processpack();
 public:
  serv();
  serv(int port, int maxconn);
  void initserver(int port, int maxconn);
  void start_server(user* user, int sizeuser);
};
}  // namespace server
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
  int size();
  void resize(int n);
  T& operator[](int index);
};
class serv {
 private:
  int sock;
  struct sockaddr_in address;
  int port;
  int maxconn;
  std::vector<pollfd> sockets;
  array<user> users;
  int nowconnect;

 private:
  void getpack();
  void processpack();

 public:
  serv();
  serv(int port, int maxconn);
  void initserver(int port, int maxconn);
  void start_server(user* user, int sizeuser);
};
}  // namespace server
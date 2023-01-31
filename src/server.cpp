#include "server.h"
template <class T>
server::array<T>::array() {
  arrptr = NULL;
  elementsize = sizeof(T);
  capacitynumber = 0;
  number = 0;
  tsize = sizeof(T);
}
template <class T>
server::array<T>::array(int number) : array() {
  this->number = number;
  capacitynumber = number;
  arrptr = (char*)malloc(elementsize * number);
}
template <class T>
server::array<T>::array(int number, int elementsize) : array() {
  number = number;
  capacitynumber = number;
  elementsize = elementsize;
  arrptr = (char*)malloc(elementsize * number);
}

template <class T>
void server::array<T>::setelementsize(int elmsize) {
  if (number != 0) {
    int sizearray = elementsize * number;
    if (elmsize < elementsize) {
      int sizear = elmsize * number;
      int r = sizearray - sizear;
      capacitynumber += r / elmsize;
    } else {
      char* ar = (char*)malloc(elmsize * capacitynumber);
      for (int i = 0; i < number; i++) {
        memcpy(&ar[i * elmsize], &arrptr[i * elementsize], elementsize);
      }
      free(arrptr);
      arrptr = ar;
    }
  }
  elementsize = elmsize;
}

template <class T>
void server::array<T>::resize(int n) {
  if (this->capacitynumber < n) {
    this->capacitynumber = n;
    char* ar = (char*)malloc(elementsize * n);
    memcpy(ar, arrptr, number * elementsize);
    free(arrptr);
    arrptr = ar;
  }

  this->number = n;
}

template <class T>
int server::array<T>::size() {
  return this->number;
}
template <class T>
T& server::array<T>::operator[](int index) {
  if (index >= number) {
    std::cout << "error index array\n";
    throw MemoryExeption("index array\n");
  } else {
    char* a = arrptr + (elementsize * index);
    T* d = (T*)a;
    return *d;
  }
}
server::serv::serv() {
  sock = -1;
  port = 9999;
  maxconn = -1;
  nowconnect = 0;
}
server::serv::serv(int port, int maxconn) : serv() {
  initserver(port, maxconn);
}
void server::serv::initserver(int port, int maxconn) {
  this->port = port;
  this->maxconn = maxconn;
}
void server::clients::initclients(int maxclients, user* user, int sizeuser) {
  users.setelementsize(sizeuser);
  sockets.resize(maxclients);
  users.resize(maxclients);
  for (int i = 0; i < maxclients; i++) {
    memcpy(&users[i], user, sizeuser);
  }
}
// void server::serv::getpack() {
//   sockets[0].fd = sock;
//   sockets[0].events = POLLIN;
//   sockets[0].revents = 0;
//   for (int i = 0; i < sockets.size(); i++) {
//     sockets[i].fd = -1;
//   }
// }
// void server::serv::processpack() {}

void server::serv::start_server(user* user, int sizeuser) {
  if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    throw NetworkExeption("SOCKET FAILD\n");
  }
  int opt = 1;
  // Forcefully attaching socket to the port 8080
  if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    throw NetworkExeption("SET SOCK OPT\n");
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
  if (bind(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
    throw NetworkExeption("bind failed\n");
  }
  if (maxconn == -1)
    maxconn = 100000;
  cls.initclients(maxconn, user, sizeuser);
  if (listen(sock, maxconn) < 0) {
    throw NetworkExeption("listen\n");
  }
}
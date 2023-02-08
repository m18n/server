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
void server::NetworkEvent::initsock(int port,
                                    int maxclients,
                                    user* user,
                                    int sizeuser) {
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
  if (maxclients == -1)
    maxclients = 100000;
  if (listen(sock, maxclients) < 0) {
    throw NetworkExeption("listen\n");
  }
  users.initusers(sock, maxclients, user, sizeuser);

  this->sock = sock;
}

void server::NetworkEvent::newpack() {
  
}
void server::NetworkEvent::addlastpack() {}
void server::NetworkEvent::getevent() {
  socklen_t addrlen = sizeof(address);

  while (1) {
    bool isconn = false;
    usersock us = users.geteventusers(&isconn);
    if (isconn) {
      // new conn
      addrlen = sizeof(address);
      int newconnect = accept(sock, (struct sockaddr*)&address, &addrlen);
      us.sock = newconnect;
      users.regusersocket(us);
    } else {
      // pack
      if (users.emptylastpack(us.iduser)) {
        // new pack
        char* headpack[8];
        int length = recv(us.sock, headpack, 8, NULL);
        if (length < 0) {
          users.disconnect(us.iduser);
        } else if (length > 0 && length < 8) {
          pack pk;
          memcpy(pk.headpack, headpack, 8);
          pk.realsize = length;
          users.addlastpack(us.iduser, pk);
        }
      } else {
        // lastpack
      }
    }
  }
}
void server::NetworkEvent::processpack() {}
void server::NetworkEvent::start() {
  std::thread gt(&NetworkEvent::getevent, this);
  gt.detach();
  std::thread pp(&NetworkEvent::processpack, this);
  pp.detach();
}
server::serv::serv() {
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

void server::serv::start_server(user* user, int sizeuser) {
  net.initsock(port, maxconn, user, sizeuser);
  net.start();
}
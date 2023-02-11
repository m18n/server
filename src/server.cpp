#include "server.h"
void server::Users::initusers(int sock,
                              int maxusers,
                              user* user,
                              int sizeuser) {
  clientusers.setelementsize(sizeuser);
  clientusers.resize(maxusers);
  serverusers.resize(maxusers);
  userssockets.resize(maxusers);
  for (int i = 0; i < maxusers; i++) {
    memcpy(&clientusers[i], user, sizeuser);
    userssockets[i].fd = -1;
    serverusers[i].iduser = i;
  }
  userssockets[0].fd = sock;
  userssockets[0].events = POLLIN;
  userssockets[0].revents = 0;
}
void server::Users::regusersocket(int sock) {
  for (int i = 0; i < userssockets.size(); i++) {
    if (userssockets[i].fd == -1) {
      if (i > uparrays)
        uparrays = i;
      userssockets[i].fd = sock;
      userssockets[i].events = POLLIN;
      userssockets[i].revents = 0;
    }
  }
}
void server::Users::geteventusers(std::vector<usersock>* retarray) {
  int ret = poll(&userssockets[0], uparrays, -1);
  if (ret < 0) {
    throw NetworkExeption("ERROR POLL");
  }
  retarray->resize(0);
  for (int i = 0; i < uparrays; i++) {
    if (userssockets[i].revents && POLLIN) {
      userssockets[i].revents &= ~POLLIN;
      usersock s;
      s.iduser = i;
      s.sock = userssockets[i].fd;
      retarray->push_back(s);
    }
  }
  if (retarray->size() == 0)
    throw NetworkExeption("NONE");
}
bool server::Users::emptylastpack(int iduser) {
  if (serverusers[iduser].lastpack.data == NULL)
    return true;

  return false;
}
void server::Users::reglastpack(int iduser, pack pk) {
  serverusers[iduser].lastpack = pk;
}
void server::Users::disconnect(int iduser) {
  // clear
  serverusers[iduser].Clear();
}
void server::Users::adddata(int iduser, int addsize) {
  serverusers[iduser].lastpack.realsize += addsize;
}
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
void server::serv::initsocket(user* user, int sizeuser) {
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

  if (listen(sock, maxconn) < 0) {
    throw NetworkExeption("listen\n");
  }
}
void server::serv::initserver(int port, int maxconn, user* user, int sizeuser) {
  this->port = port;
  this->maxconn = maxconn;
  if (maxconn == -1)
    maxconn = 100000;
  initsocket(user, sizeuser);
  users.initusers(sock, maxconn, user, sizeuser);
  this->init = true;
}
server::serv::serv() {
  port = 9999;
  maxconn = -1;
  nowconnect = 0;
  init = false;
}
server::serv::serv(int port, int maxconn, user* user, int sizeuser) : serv() {
  initserver(port, maxconn, user, sizeuser);
}

void server::serv::processpack() {}
void server::serv::getevent() {
  socklen_t addrlen = sizeof(address);
  std::vector<usersock> arrus;
  usersock* us;
  while (1) {
    users.geteventusers(&arrus);
    for (int i = 0; i < arrus.size(); i++) {
      if (arrus[i].iduser == 0) {
        // new conn
        addrlen = sizeof(address);
        int sock = accept(sock, (struct sockaddr*)&address, &addrlen);
        users.regusersocket(sock);
      } else {
        us = &arrus[i];
        // pack
        if (users.emptylastpack(us->iduser)) {
          // new pack
          char* headpack[8];
          int length = recv(us->sock, headpack, 8, NULL);
          if (length < 0) {
            users.disconnect(us->iduser);
          } else if (length > 0 && length < 8) {
            pack pk;
            memcpy(pk.headpack, headpack, 8);
            pk.realsize = length;
            users.reglastpack(us->iduser, pk);
          }
        } else {
          // lastpack
        }
      }
    }
  }
}

void server::serv::start_server() {
  if (init) {
    std::thread gt(&serv::getevent, this);
    gt.detach();
    std::thread pp(&serv::processpack, this);
    pp.detach();
  }
}
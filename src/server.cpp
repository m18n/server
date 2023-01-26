#include "server.h"

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
void server::serv::start_server() {
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
  int r = 0;
  if (maxconn == -1) {
    sockets.resize(100000);
    r = listen(sock, 100000);
  } else if (maxconn > 0) {
    sockets.resize(maxconn);
    r = listen(sock, maxconn);
  }
  if (r < 0) {
    throw NetworkExeption("listen\n");
  }
}
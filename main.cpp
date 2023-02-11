#include <stdio.h>

#include <pthread.h>
#include "server.h"
using namespace server;
class olxclient : public user {
  int a = 0;

 public:
  olxclient() { a = 10; }
  int geta() { return a; }
};
int main() {
  serv sv(9999,3,new olxclient(),sizeof(olxclient));
  sv.start_server();
  while (true)
  {
    
  }
  
  
  return 0;
}
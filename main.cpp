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
  serv sv;

  sv.initserver(9999, 3);
  sv.start_server(new olxclient(), sizeof(olxclient));
  while(true){
    
  }
  // sv.users.resize(2);
  return 0;
}
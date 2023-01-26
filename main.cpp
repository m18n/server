#include<stdio.h>

#include <pthread.h>
#include"server.h"
using namespace server;
int main(){
    serv sv;
    sv.initserver(2,0);
    sv.start_server();
    return 0;
}
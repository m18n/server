#include<stdio.h>

#include <pthread.h>
#include"server.h"
using namespace server;
class olxclient:public user{
 int a=0;   
};
int main(){
    serv sv;
    sv.initserver(9999,3);
    sv.start_server(new olxclient,sizeof(olxclient));
    
    
    return 0;
}
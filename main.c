#include<stdio.h>

#include <pthread.h>
#include"server.h"
sv_server_t serv;
int main(){
    pthread_t process_pack;
    printf("HELLO ");
    
    sv_ServerInit(&serv,9999,10);
    //InitUserPack();
    sv_v2_t v;
    v.p1=&serv.arrpack;
    pthread_create( &process_pack, NULL, sv_arraypack_process, (void*) &v);
    //GetEventsSocket(&serv,sizeof(olxuser_t),CreateOlxUser,ClearOlxUser);
    return 0;
}
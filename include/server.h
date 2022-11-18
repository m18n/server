#pragma once

#include"parser.h"
#include<stdbool.h>
#include"packs.h"
#include"core.h"
typedef struct arraypack{
    pack_t* packs;
    bool* getpaks;
    int sizepacks;
    pthread_mutex_t mtpack;
    int upper;
}arraypack_t;
void CreateArrayPack(arraypack_t* array);
void InitArrayPack(arraypack_t* array,int sizepack);
pack_t* arraypack_getelement(arraypack_t* array);
void arraypack_finishpack(arraypack_t* array,pack_t* pack);
void arraypack_process(v2_t* v);

typedef struct server{
    int sock_f;
    struct sockaddr_in address;
    int port;
    int opt;
    int maxconn;
    int sizesockets;
    struct pollfd* sockets;
    user_packs_t* users;
    arraypack_t arrpack;
    arrayd_t userpacks;
    
    int nowconnect;
}server_t;
void ServerInit(server_t* serv,int port,int maxconn);
packreq_t* serv_getpackreq(server_t* serv,int idpack);
void serv_adduserpacks(server_t* serv,void(*CreatePack)(packreq_t* self),int sizeuserpack,int idpack);
void GetEventsSocket(server_t* serv,int sizeuser,void(*CreateUsere)(user_t* us),void(*ClearUser)(user_t*us));



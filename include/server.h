#pragma once

#include"parser.h"
#include<stdbool.h>
#include"packs.h"
#include"core.h"
typedef struct sv_arraypack{
    sv_pack_t* packs;
    bool* getpaks;
    int sizepacks;
    pthread_mutex_t mtpack;
    int upper;
}sv_arraypack_t;
void sv_CreateArrayPack(sv_arraypack_t* array);
void sv_InitArrayPack(sv_arraypack_t* array,int sizepack);
sv_pack_t* sv_arraypack_getelement(sv_arraypack_t* array);
void sv_arraypack_finishpack(sv_arraypack_t* array,sv_pack_t* pack);
void sv_arraypack_process(sv_arraypack_t* array);

typedef struct sv_server{
    int sock_f;
    struct sockaddr_in address;
    int port;
    int opt;
    int maxconn;
    int sizesockets;
    struct pollfd* sockets;
    sv_user_packs_t* users;
    sv_arraypack_t arrpack;
    sv_arrayd_t userpacks;
    
    int nowconnect;
}sv_server_t;
void sv_ServerInit(sv_server_t* serv,int port,int maxconn);
sv_packreq_t* sv_serv_getpackreq(sv_server_t* serv,int idpack);
void sv_serv_adduserpacks(sv_server_t* serv,void(*CreatePack)(sv_packreq_t* self),int sizeuserpack,int idpack);
void sv_GetEventsSocket(sv_server_t* serv,int sizeuser,void(*CreateUsere)(sv_user_t* us),void(*ClearUser)(sv_user_t*us));
void sv_ServerStart(sv_server_t* serv,int sizeuser,void(*CreateUsere)(sv_user_t* us),void(*ClearUser)(sv_user_t*us));

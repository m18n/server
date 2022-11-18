#pragma once
#include<stdio.h>
#include<stdbool.h>
#include"parser.h"
#include"core.h"
typedef struct packreq{
    int indexpack;
    bool(*JsonToObject)(struct packreq* pack,json_value* json);
    void(*ProcessPack)(struct packreq* pack,user_t* user);
}packreq_t;
void CreatePackReq(packreq_t* pack);
json_construct_t GetPackJson(int idpack);
json_construct_t GetPackJsonRes(int idpack,int indexpack);
typedef struct packres{
    int idpack;
    json_construct_t(*GetJsonPack)(struct packres* pk);
}packres_t;
void CreatePackRes(packres_t* pack);
int sendall(int sock_conn, char *buf, int *len);
void SendPack(user_t* user,packres_t* pk);
void SendPackRes(user_t* user,packres_t* pk,packreq_t* pkreq);

typedef struct infopackreq{
    int sizepack;
    int idpack;
    void(*CreatePack)(packreq_t* req);
}infopackreq_t;
void CreateInfoPackReq(infopackreq_t* self);


typedef struct pack
{
    int realsizepack;
    int allsizepack;
    char* data;
    json_value* json;
    bool compile;
    user_t* user;
    packreq_t* pack_req;
    int idpack;
}pack_t;

void auth(pack_t* pk);
void CreatePack(pack_t* pack);
void InitPack(pack_t* pack,char* data,int realsize,int allsize,json_value* values,user_t* user);
void DestroyPack(pack_t* pack);


typedef struct user_packs{
    user_t* user;
    pack_t* lastpack;
  
}user_packs_t;
void CreateUserPacks(user_packs_t* user);
void InitUserPacks(user_packs_t* user,int sizeuser,void(*CUser)(user_t* user));
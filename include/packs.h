#pragma once
#include<stdio.h>
#include<stdbool.h>
#include"parser.h"
#include"core.h"
typedef struct sv_packreq{
    int indexpack;
    bool(*JsonToObject)(struct sv_packreq* pack,json_value* json);
    void(*ProcessPack)(struct sv_packreq* pack,sv_user_t* user);
}sv_packreq_t;
void sv_CreatePackReq(sv_packreq_t* pack);
sv_json_construct_t sv_GetPackJson(int idpack);
sv_json_construct_t sv_GetPackJsonRes(int idpack,int indexpack);
typedef struct sv_packres{
    int idpack;
    sv_json_construct_t(*GetJsonPack)(struct sv_packres* pk);
}sv_packres_t;
void sv_CreatePackRes(sv_packres_t* pack);
int sv_sendall(int sock_conn, char *buf, int *len);
void sv_SendPack(sv_user_t* user,sv_packres_t* pk);
void sv_SendPackRes(sv_user_t* user,sv_packres_t* pk,sv_packreq_t* pkreq);

typedef struct sv_infopackreq{
    int sizepack;
    int idpack;
    void(*CreatePack)(sv_packreq_t* req);
}sv_infopackreq_t;
void sv_CreateInfoPackReq(sv_infopackreq_t* self);


typedef struct sv_pack
{
    int realsizepack;
    int allsizepack;
    char* data;
    json_value* json;
    bool compile;
    sv_user_t* user;
    sv_packreq_t* pack_req;
    int idpack;
}sv_pack_t;

void sv_auth(sv_pack_t* pk);
void sv_CreatePack(sv_pack_t* pack);
void sv_InitPack(sv_pack_t* pack,char* data,int realsize,int allsize,json_value* values,sv_user_t* user);
void sv_DestroyPack(sv_pack_t* pack);


typedef struct sv_user_packs{
    sv_user_t* user;
    sv_pack_t* lastpack;
  
}sv_user_packs_t;
void sv_CreateUserPacks(sv_user_packs_t* user);
void sv_InitUserPacks(sv_user_packs_t* user,int sizeuser,void(*CUser)(sv_user_t* user));
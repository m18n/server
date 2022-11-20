#include"packs.h"
void sv_CreateUserPacks(sv_user_packs_t* user){
    user->user=NULL;
    user->lastpack=NULL;
    
}
void sv_CreateInfoPackReq(sv_infopackreq_t* self){
    self->CreatePack=NULL;
    self->sizepack=0;
    self->idpack=0;
}
void sv_InitUserPacks(sv_user_packs_t* user,int sizeuser,void(*CUser)(sv_user_t* user)){
    sv_CreateUserPacks(user);
    user->user=malloc(sizeuser);
    sv_CreateUser(user->user);
    CUser(user->user);
    
}

void sv_CreatePack(sv_pack_t* pack){
    if(pack!=NULL){
    pack->allsizepack=0;
    pack->data=NULL;

    pack->realsizepack=0;
    pack->compile=false;
    pack->json=NULL;
    pack->user=NULL;
    pack->pack_req=NULL;
    pack->idpack=-1;
    }
}
void sv_InitPack(sv_pack_t* pack,char* data,int realsize,int allsize,json_value* values,sv_user_t* user){
    sv_CreatePack(pack);
    pack->allsizepack=allsize;
    pack->realsizepack=realsize;
    pack->data=data;
    pack->json=values;
    pack->user=user;
}
int sv_sendall(int sock_conn, char *buf, int *len)
{
 int total = 0; // сколько байт мы послали
 int bytesleft = *len; // сколько байт осталось послать
 int n;
 while(total < *len) {
 n = send(sock_conn, buf+total, bytesleft, MSG_NOSIGNAL);
 if (n == -1) { break; }
 total += n;
 bytesleft -= n;
 }
 *len = total; // здесь количество действительно посланных байт
 return n==-1?-1:0; // вернуть -1 при сбое, 0 при успехе
}
sv_json_construct_t sv_GetPackJson(int idpack){
    sv_json_construct_t c;
    sv_CreateJson_Construct(&c);
    sv_json_item_t jidpack;
    sv_CreateJson_Item(&jidpack);
    sv_json_item_setname(&jidpack,"idpack");
    sv_json_item_setintvalue(&jidpack,idpack);
    sv_json_construct_addelement(&c,jidpack);
    return c;
}
sv_json_construct_t sv_GetPackJsonRes(int idpack,int indexpack){
    sv_json_construct_t c;
    sv_CreateJson_Construct(&c);
    sv_json_item_t jidpack;
    sv_CreateJson_Item(&jidpack);
    sv_json_item_setname(&jidpack,"idpack");
    sv_json_item_setintvalue(&jidpack,idpack);
    sv_json_construct_addelement(&c,jidpack);
    sv_json_item_t jindexpack;
    sv_CreateJson_Item(&jindexpack);
    sv_json_item_setname(&jindexpack,"indexpack");
    sv_json_item_setintvalue(&jindexpack,indexpack);
    sv_json_construct_addelement(&c,jindexpack);
    return c;
}
void sv_SendPack(sv_user_t* user,sv_packres_t* pk){
    sv_json_construct_t con=pk->GetJsonPack(pk);
    sv_json_item_t jidpack;
    sv_CreateJson_Item(&jidpack);
    sv_json_item_setname(&jidpack,"idpack");
    sv_json_item_setintvalue(&jidpack,pk->idpack);
    sv_json_construct_addelement(&con,jidpack);
    int size=0;
    char* c=sv_json_construct_getstring_SEND(&con,&size);
    sv_sendall(user->pollptr->fd,c,&size);
    free(c);
    sv_DestroyJson_Construct(&con);
}
void sv_SendPackRes(sv_user_t* user,sv_packres_t* pk,sv_packreq_t* pkreq){
    sv_json_construct_t con=pk->GetJsonPack(pk);
    sv_json_item_t jidpack;
    sv_CreateJson_Item(&jidpack);
    sv_json_item_setname(&jidpack,"idpack");
    sv_json_item_setintvalue(&jidpack,pk->idpack);
    sv_json_construct_addelement(&con,jidpack);
    sv_json_item_t jindexpack;
    sv_CreateJson_Item(&jindexpack);
    sv_json_item_setname(&jindexpack,"indexpack");
    sv_json_item_setintvalue(&jindexpack,pkreq->indexpack);
    sv_json_construct_addelement(&con,jindexpack);
    int size=0;
    char* c=sv_json_construct_getstring_SEND(&con,&size);
    sv_sendall(user->pollptr->fd,c,&size);
    free(c);
    sv_DestroyJson_Construct(&con);
}
void sv_SendPackResIndex(sv_user_t* user,sv_packres_t* pk,int indexpack){
    sv_json_construct_t con=pk->GetJsonPack(pk);
    sv_json_item_t jidpack;
    sv_CreateJson_Item(&jidpack);
    sv_json_item_setname(&jidpack,"idpack");
    sv_json_item_setintvalue(&jidpack,pk->idpack);
    sv_json_construct_addelement(&con,jidpack);
    sv_json_item_t jindexpack;
    sv_CreateJson_Item(&jindexpack);
    sv_json_item_setname(&jindexpack,"indexpack");
    sv_json_item_setintvalue(&jindexpack,indexpack);
    sv_json_construct_addelement(&con,jindexpack);
    int size=0;
    char* c=sv_json_construct_getstring_SEND(&con,&size);
    sv_sendall(user->pollptr->fd,c,&size);
    free(c);
    sv_DestroyJson_Construct(&con);
}
void sv_DestroyPack(sv_pack_t* pack){
    if(pack->data!=NULL)
        free(pack->data);
    if(pack->json!=NULL)
        json_value_free(pack->json);
    if(pack->pack_req!=NULL)
        free(pack->pack_req);
    sv_CreatePack(pack);
}
void sv_auth(sv_pack_t* pk){
    printf("AUTH\n");
    json_object_entry*nobj= sv_GetNameKey(pk->json,"name");
    json_object_entry*pobj=sv_GetNameKey(pk->json,"password");
    char* name=nobj->value->u.string.ptr;
    char* password=pobj->value->u.string.ptr;
    if(strcmp(name,"misha")==0){
        if(strcmp(password,"123")==0){
            printf("NAME AND PASSWORD OK\n");
        }
    }
    
}

void sv_CreatePackReq(sv_packreq_t* pack){
    pack->indexpack=-1;
    pack->JsonToObject=NULL;
    pack->ProcessPack=NULL;
}
void sv_CreatePackRes(sv_packres_t* pack){
    pack->idpack=-1;
    pack->GetJsonPack=NULL;
}

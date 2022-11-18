#include"packs.h"
void CreateUserPacks(user_packs_t* user){
    user->user=NULL;
    user->lastpack=NULL;
    
}
void CreateInfoPackReq(infopackreq_t* self){
    self->CreatePack=NULL;
    self->sizepack=0;
    self->idpack=0;
}
void InitUserPacks(user_packs_t* user,int sizeuser,void(*CUser)(user_t* user)){
    CreateUserPacks(user);
    user->user=malloc(sizeuser);
    CreateUser(user->user);
    CUser(user->user);
    
}

void CreatePack(pack_t* pack){
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
void InitPack(pack_t* pack,char* data,int realsize,int allsize,json_value* values,user_t* user){
    CreatePack(pack);
    pack->allsizepack=allsize;
    pack->realsizepack=realsize;
    pack->data=data;
    pack->json=values;
    pack->user=user;
}
int sendall(int sock_conn, char *buf, int *len)
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
json_construct_t GetPackJson(int idpack){
    json_construct_t c;
    CreateJson_Construct(&c);
    json_item_t jidpack;
    CreateJson_Item(&jidpack);
    json_item_setname(&jidpack,"idpack");
    json_item_setintvalue(&jidpack,idpack);
    json_construct_addelement(&c,jidpack);
    return c;
}
json_construct_t GetPackJsonRes(int idpack,int indexpack){
    json_construct_t c;
    CreateJson_Construct(&c);
    json_item_t jidpack;
    CreateJson_Item(&jidpack);
    json_item_setname(&jidpack,"idpack");
    json_item_setintvalue(&jidpack,idpack);
    json_construct_addelement(&c,jidpack);
    json_item_t jindexpack;
    CreateJson_Item(&jindexpack);
    json_item_setname(&jindexpack,"indexpack");
    json_item_setintvalue(&jindexpack,indexpack);
    json_construct_addelement(&c,jindexpack);
    return c;
}
void SendPack(user_t* user,packres_t* pk){
    json_construct_t con=pk->GetJsonPack(pk);
    json_item_t jidpack;
    CreateJson_Item(&jidpack);
    json_item_setname(&jidpack,"idpack");
    json_item_setintvalue(&jidpack,pk->idpack);
    json_construct_addelement(&con,jidpack);
    int size=0;
    char* c=json_construct_getstring_SEND(&con,&size);
    sendall(user->pollptr->fd,c,&size);
    free(c);
    DestroyJson_Construct(&con);
}
void SendPackRes(user_t* user,packres_t* pk,packreq_t* pkreq){
    json_construct_t con=pk->GetJsonPack(pk);
    json_item_t jidpack;
    CreateJson_Item(&jidpack);
    json_item_setname(&jidpack,"idpack");
    json_item_setintvalue(&jidpack,pk->idpack);
    json_construct_addelement(&con,jidpack);
    json_item_t jindexpack;
    CreateJson_Item(&jindexpack);
    json_item_setname(&jindexpack,"indexpack");
    json_item_setintvalue(&jindexpack,pkreq->indexpack);
    json_construct_addelement(&con,jindexpack);
    int size=0;
    char* c=json_construct_getstring_SEND(&con,&size);
    sendall(user->pollptr->fd,c,&size);
    free(c);
    DestroyJson_Construct(&con);
}
void DestroyPack(pack_t* pack){
    if(pack->data!=NULL)
        free(pack->data);
    if(pack->json!=NULL)
        json_value_free(pack->json);
    if(pack->pack_req!=NULL)
        free(pack->pack_req);
    CreatePack(pack);
}
void auth(pack_t* pk){
    printf("AUTH\n");
    json_object_entry*nobj= GetNameKey(pk->json,"name");
    json_object_entry*pobj=GetNameKey(pk->json,"password");
    char* name=nobj->value->u.string.ptr;
    char* password=pobj->value->u.string.ptr;
    if(strcmp(name,"misha")==0){
        if(strcmp(password,"123")==0){
            printf("NAME AND PASSWORD OK\n");
        }
    }
    
}

void CreatePackReq(packreq_t* pack){
    pack->indexpack=-1;
    pack->JsonToObject=NULL;
    pack->ProcessPack=NULL;
}
void CreatePackRes(packres_t* pack){
    pack->idpack=-1;
    pack->GetJsonPack=NULL;
}

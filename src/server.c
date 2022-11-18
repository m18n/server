#include"server.h"

void ServerInit(server_t* serv,int port,int maxconn){
    InitArrayPack(&serv->arrpack,10000);
    serv->sizesockets=maxconn+1;
    serv->nowconnect=0;
    InitArrayd(&serv->userpacks,10,sizeof(infopackreq_t));
    serv->sockets=malloc(serv->sizesockets*sizeof(struct pollfd));
    serv->opt=1;
    serv->port=port;
    serv->users=malloc(serv->sizesockets*sizeof(user_packs_t));
    serv->maxconn=maxconn;
    if ((serv->sock_f = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(1);
    }
 
    // Forcefully attaching socket to the port 8080
    if (setsockopt(serv->sock_f, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &serv->opt,
                   sizeof(serv->opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    serv->address.sin_family = AF_INET;
    serv->address.sin_addr.s_addr = INADDR_ANY;
    serv->address.sin_port = htons(serv->port);
    if (bind(serv->sock_f, (struct sockaddr*)&serv->address,
             sizeof(serv->address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(serv->sock_f, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    
    printf("CONNECT\n");
}
void serv_adduserpacks(server_t* serv,void(*CreatePack)(packreq_t* self),int sizeuserpack,int idpack){
    infopackreq_t info;
    info.CreatePack=CreatePack;
    info.idpack=idpack;
    info.sizepack=sizeuserpack;
    arrayd_addelement(&serv->userpacks,&info);
}
packreq_t* serv_getpackreq(server_t* serv,int idpack){
    for(int i=0;i<serv->userpacks.realsize;i++){
        infopackreq_t* info=&serv->userpacks.data[i*sizeof(infopackreq_t)];
        if(info->idpack==idpack){
            packreq_t* pack=malloc(info->sizepack);
            info->CreatePack(pack);
            return pack;
        }
    }
    return NULL;
}
void CreateArrayPack(arraypack_t* array){
    array->packs=NULL;
    array->sizepacks=0;
    array->upper=0;
}
void InitArrayPack(arraypack_t* array,int sizepack){
    CreateArrayPack(array);
    pthread_mutex_init(&array->mtpack, NULL);
    array->sizepacks=sizepack;
    array->upper=1;
    array->packs=malloc(sizeof(pack_t)*sizepack);
    array->getpaks=malloc(sizeof(bool)*sizepack);
    for(int i=0;i<sizepack;i++){
        CreatePack(&array->packs[i]);
    }
}
pack_t* arraypack_getelement(arraypack_t* array){
    bool search=false;
    pthread_mutex_lock(&array->mtpack);
    for(int i=0;i<array->upper;i++){
        bool* pack=&array->getpaks[i];
        if(array->getpaks[i]==false){
            search=true;
            printf("INDEX ADD PACKS: %d UPPER: %d =\n",i,array->upper);
            array->getpaks[i]=true;
            pthread_mutex_unlock(&array->mtpack);
            return &array->packs[i];
        }
    }
    if(search==false){
        for(int i=array->upper;i<array->sizepacks;i++){
            bool* pack=&array->getpaks[i];
            if(array->getpaks[i]==false){
                array->upper=i+1;
                printf("INDEX ADD PACKS: %d UPPER: %d +\n",i,array->upper);
                array->getpaks[i]=true;
                pthread_mutex_unlock(&array->mtpack);
                return &array->packs[i];
            }
        }
    }
    pthread_mutex_unlock(&array->mtpack);
    return NULL;
}
void arraypack_finishpack(arraypack_t* array,pack_t* pack){
    if(pack!=NULL){
        int index=pack-array->packs;
        DestroyPack(&array->packs[index]);
        pthread_mutex_lock(&array->mtpack);
       // printf("\nFINISH: %d Up=%d\n\n",index,array->upper);
        array->getpaks[index]=false;
        if(index+1==array->upper){
            bool s=false;
            for(int i=array->upper;i>0;i--){
                if(array->getpaks[i]==true){
                    s=true;
                    array->upper=i+1;
                    //printf("\nUPPER=%d -\n\n",array->upper);
                    break;
                }
            }
            if(s==false){
                printf("\nUPPER NULL\n\n");
                array->upper=1;
            }
        }
        pthread_mutex_unlock(&array->mtpack);
    }
}
void arraypack_process(v2_t* v){
    arraypack_t* array=v->p1;
    printf("START PROCESS PACK\n");
    while (true)
    {
        for(int i=0;i<array->sizepacks;i++){
            if(array->getpaks[i]==true){
                if(array->packs[i].compile==true){
                    printf("PROCESS PACK\n");
                    pack_t* p=&array->packs[i];
                    if(p->pack_req!=NULL)
                        p->pack_req->ProcessPack(p->pack_req,p->user);
                    arraypack_finishpack(array,p);
                     
                   
                }
            }
        }
    }
}

void GetEventsSocket(server_t* serv,int sizeuser,void(*CreateUsere)(user_t* us),void(*ClearUser)(user_t*us)){
    int addrlen=sizeof(serv->address);
    int conn=-1;
    serv->sockets[0].fd=serv->sock_f;
    serv->sockets[0].events=POLLIN;
    serv->sockets[0].revents=0;
    
    for(int i=1;i<serv->sizesockets;i++){
        serv->sockets[i].fd=-1;
        InitUserPacks(&serv->users[i],sizeuser,CreateUsere);
    }
    while(1){
        int ret=poll(serv->sockets,serv->sizesockets,-1);
        if(ret<0){
            perror("Server: poll fatal");
            exit(1);
        }
        else if(ret>0){
            for(int i=0;i<serv->sizesockets;i++){
                if(serv->sockets[i].revents&&POLLIN){
                    serv->sockets[i].revents&=~POLLIN;
                    if(i==0){
                        //NEW CONNECT
                        
                        addrlen=sizeof(serv->address);
                        int newconn=accept(serv->sockets[i].fd,(struct sockaddr*)&serv->address,&addrlen);
                     
                        bool search=false;
                        for(int j=1;j<serv->sizesockets;j++){
                            if(serv->sockets[j].fd==-1){
                                printf("INDEX CLINET: %d\n",j);
                                
                                serv->sockets[j].fd=newconn;
                                serv->sockets[j].events=POLLIN;
                                serv->sockets[j].revents=0;
                                user_t* us=serv->users[j].user;
                                serv->users[j].lastpack=NULL;
                                CreateUser(serv->users[j].user);
                                CreateUsere(serv->users[j].user);
                                serv->users[j].user->pollptr=&serv->sockets[j];
                                search=true;
                                
                                break;
                            }
                        }
                        if(search==false){
                            printf("MAX CONNET\n");
                            close(newconn);
                        }else{
                            serv->nowconnect++;
                            printf("NOW CONNECT: %d\n",serv->nowconnect);
                        }
                        
                        
                        
                    }else{
                        //RECV DATA
                        serv->sockets[i].revents&=~POLLIN;
                        int length=-1;
                        if(serv->users[i].lastpack==NULL){
                            //новий пакет
                            char* bufferheadpack[4];
                            length=recv(serv->sockets[i].fd,bufferheadpack,4,NULL);
                            int sizepack=-1;
                            if(length==4){
                                
                                memcpy(&sizepack,bufferheadpack,4);
                                char* data=malloc(sizepack);
                                    
                                length=recv(serv->sockets[i].fd,data,sizepack,NULL);
                                if(length>0){
                                    // дані є
                                    pack_t* pack=arraypack_getelement(&serv->arrpack);
                                    if(pack==NULL){
                                        //немає місця для пакету
                                        printf("MAX PACK\n");
                                        free(data);
                                      
                                    }else{
                                        
                                        printf("DATA: %s REALSIZE: %d ALLSIZEPACK: %d\n",data,length,sizepack);
                                        if(sizepack!=length){
                                            InitPack(pack,data,length,sizepack,NULL,serv->users[i].user);
                                            serv->users[i].lastpack=pack;
                                        }else{
                                            //повний пакет
                                            char* json=(json_char*)data;
                                            json_value* value = json_parse(json,sizepack);
                                            if(value!=NULL){
                                                json_object_entry* type=GetNameKey(value,"idpack");
                                                json_object_entry* index=GetNameKey(value,"indexpack");
                                                if(type!=NULL&&index!=NULL&&index->value->type==json_integer&&type->value->type==json_integer) {
                                                    int idpack=type->value->u.integer;
                                                    int indexpack=index->value->u.integer;
                                                    InitPack(pack,data,length,sizepack,value,serv->users[i].user);
                                                    
                                                    //SetPackFunction(pack,indextype);
                                                    pack->pack_req=serv_getpackreq(serv,idpack);
                                                    if(pack->pack_req!=NULL){
                                                        bool r=pack->pack_req->JsonToObject(pack->pack_req,value);
                                                        if(r==true){
                                                            pack->pack_req->indexpack=indexpack;
                                                            pack->compile=true;
                                                        }else{
                                                            printf("ERROR PARSE PACK\n");
                                                            length=0;
                                                            arraypack_finishpack(&serv->arrpack,pack);
                                                        }
                                                    }
                                                }else{
                                                    printf("ERROR ID PACK\n");
                                                    length=0;
                                                    
                                                    arraypack_finishpack(&serv->arrpack,pack);
                                                }
                                            }else{
                                                printf("ERROR PARSE JSON\n");
                                                length=0;
                                                arraypack_finishpack(&serv->arrpack,pack);
                                            }
                                            
                                        }
                                    }
                                    
                                }else{
                                    // даних немає
                                    free(data);
                                }
                            }
                          
                        }else{
                            // додати дані до минулого пакету
                            pack_t* lastpack=serv->users[i].lastpack;
                            if(lastpack->allsizepack>lastpack->realsizepack){
                                int size=lastpack->allsizepack-lastpack->realsizepack;
                                char* start=&lastpack->data[lastpack->realsizepack];
                                length=recv(serv->users[i].user->pollptr->fd,start,size,NULL);
                                if(length>0){
                                    lastpack->realsizepack+=length;
                                    if(lastpack->realsizepack==lastpack->allsizepack){
                                        //повний пакет
                                        char* json=(json_char*)serv->users[i].lastpack->data;
                                        json_value* value = json_parse(json,serv->users[i].lastpack->realsizepack);
                                        if(value!=NULL){
                                            json_object_entry* jidpack=GetNameKey(value,"idpack");
                                            json_object_entry* jindex=GetNameKey(value,"indexpack");
                                            if(jidpack!=NULL&&jindex!=NULL&&jindex->value->type==json_integer&&jidpack->value->type==json_integer) {
                                                int idpack=jidpack->value->u.integer;
                                                int indexpack=jindex->value->u.integer;
                                                //SetPackFunction(serv->users[i].lastpack,indextype);
                                                serv->users[i].lastpack->json=value;
                                                serv->users[i].lastpack->pack_req=serv_getpackreq(serv,idpack);
                                                if(serv->users[i].lastpack->pack_req!=NULL){
                                                        bool r=serv->users[i].lastpack->pack_req->JsonToObject(serv->users[i].lastpack->pack_req,value);
                                                    if(r==true){
                                                        serv->users[i].lastpack->pack_req->indexpack=indexpack;
                                                        serv->users[i].lastpack->compile=true;
                                                        serv->users[i].lastpack=NULL; 
                                                    }else{
                                                        printf("ERROR PARSE PACK\n");
                                                        length=0;
                                                    }
                                                }
                                                
                                                                             
                                            }else{
                                                printf("ERROR ID TYPE\n");
                                                length=-1;     
                                            }
                                        }else{
                                            printf("ERROR PARSE JSON\n");
                                            length=-1;
                                        }
                                       
                                    }
                                    printf("TWO: %s\n",start);
                                    printf("ALL: %s\n",lastpack->data);
                                }
                            }
                        }
                        if(length==0){
                                printf("Disconnect\n\n\n");
                                serv->nowconnect--;
                               // int d=u->user->pollptr->fd;
                                user_disconnect(serv->users[i].user,ClearUser);
                                //d=u->user->pollptr->fd;
                                //cleardata
                                if(serv->users[i].lastpack!=NULL){
                                    arraypack_finishpack(&serv->arrpack,serv->users[i].lastpack);
                                }

                        }else if(length<0){
                                printf("ERROR READ PACK\n");
                                serv->nowconnect--;
                                user_disconnect(serv->users[i].user,ClearUser);
                                
                                //cleardata
                                 if(serv->users[i].lastpack!=NULL){
                                    arraypack_finishpack(&serv->arrpack,serv->users[i].lastpack);
                                }                           
                        }
                    }
                }
            }
        }
    }
}
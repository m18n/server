#include"core.h"

void CreateUser(user_t* user){
    user->pollptr=NULL;
  
}
void user_disconnect(user_t* user,void(*ClearUser)(user_t* user)){
    close(user->pollptr->fd);
    user->pollptr->fd=-1;
    ClearUser(user);
    
}
void CreateList(list_t* list){
    list->sizelist=0;
    list->start=NULL;
  
}
void AddElementList(list_t* list,void* data,int sizedata){
    
    node_t* index=list->start;
    node_t* prev=NULL;
    for(int i=0;i<list->sizelist;i++){
        if(index->next==NULL){
            prev=index;
            index=index->next;
            break; 
        }
        
    }
    if(index==NULL){
        index=malloc(sizeof(node_t));
        index->sizedata=sizedata;
        index->next=NULL;
        index->data=data;
    }
    if(prev!=NULL)
        prev->next=index;
    if(list->start==NULL)
        list->start=index;
    list->sizelist++;
}
node_t* list_getlast(list_t* list){
    node_t* index=list->start;
    for(int i=0;i<list->sizelist;i++){
        if(index->next!=NULL){
            index=index->next;
        }else{
            break;
        }
        
    }
    return index;
}
void CreateArrayD(arrayd_t* array){
    array->allsize=0;
    array->realsize=0;
    array->data=NULL;
    array->sizeelement=0;
}
void InitArrayd(arrayd_t* array,int sizearray,int sizeelement){
    CreateArrayD(array);
    array->data=malloc(sizeelement*sizearray);
    array->allsize=sizearray;
    array->sizeelement=sizeelement;
}
void arrayd_addelement(arrayd_t* array,void* element){
    if(array->allsize==array->realsize){
        array->allsize++;
        void* newarr=malloc(array->allsize*array->sizeelement);
        memcpy(newarr,array->data,array->realsize*array->sizeelement);
        void*el=newarr+array->realsize*array->sizeelement;
        memcpy(el,element,array->sizeelement);
        array->realsize++;
        free(array->data);
        array->data=newarr;
    }else{
        void* index=array->data+array->sizeelement*array->realsize;
        memcpy(index,element,array->sizeelement);
        array->realsize++;
    }
}
void DestroyArrayD(arrayd_t* array){
    free(array->data);
}
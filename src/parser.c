#include"parser.h"
int sv_SearchNameKey(json_value* json,const char* name){
    for(int i=0;i<json->u.object.length;i++){
        if(strcmp(json->u.object.values[i].name,name)==0){
            return i;
        }
    } 
    return -1;
}
json_object_entry* sv_GetNameKey(json_value* json,const char* name){
    for(int i=0;i<json->u.object.length;i++){
        if(strcmp(json->u.object.values[i].name,name)==0){
            return &json->u.object.values[i];
        }
    } 
    return NULL;
}
bool sv_CheckKeyAndValue(json_value* json,const char* key,json_type type){
    int index=sv_SearchNameKey(json,key);
    if(index==-1)
        return false;
    if(json->u.object.values[index].value->type==type)
        return true;
    return false;   
}
void sv_CreateJson_Item(sv_json_item_t* item){
    item->name[0]='\0';
    item->strvalue=NULL;
    item->intvalue=0;
    item->bint=false;
}
void sv_json_item_setname(sv_json_item_t* item,const char* name){
    strcpy(item->name,name);
}
void sv_json_item_setintvalue(sv_json_item_t* item,int value){
    item->intvalue=value;
    item->bint=true;
}
void sv_json_item_setstrvalue(sv_json_item_t* item,const char* value){
    int size=strlen(value)+1;
    item->strvalue=malloc(size);
    strcpy(item->strvalue,value);
    item->strvalue[size-1]='\0';
}
void sv_CreateJson_Construct(sv_json_construct_t* json){
    sv_InitArrayd(&json->jsonitem,0,sizeof(sv_json_item_t));
}
void sv_DestroyJson_Construct(sv_json_construct_t* json){
    sv_json_item_t* items=json->jsonitem.data;
    for(int i=0;i<json->jsonitem.realsize;i++){
        if(items[i].strvalue!=NULL){
            free(items[i].strvalue);
        }
    }
    sv_DestroyArrayD(&json->jsonitem);
}
void sv_json_construct_addelement(sv_json_construct_t* json,sv_json_item_t item){
    sv_arrayd_addelement(json,&item);
}

char* sv_json_construct_getstring(sv_json_construct_t* json_c){
    int allsize=2;
    
    sv_json_item_t* array=json_c->jsonitem.data;
    
    for(int i=0;i<json_c->jsonitem.realsize;i++){
        allsize+=5+strlen(array[i].name);//5=|"":, |
        if(array[i].bint!=false){
            char intvaluestr[10];
            sprintf(intvaluestr, "%d",array[i].intvalue);
            allsize+=strlen(intvaluestr);
        }else{
            allsize+=2+strlen(array[i].strvalue);//2=|""|
        }
    }
    allsize-=2;//last |, |
    char* json=malloc(allsize+1);
    json[0]='\0';
    int indexjson=strlen(json);
    strcpy(&json[indexjson],"{");
    for(int i=0;i<json_c->jsonitem.realsize;i++){
        indexjson=strlen(json);
        strcpy(&json[indexjson],"\"");
        indexjson=strlen(json);
        strcpy(&json[indexjson],array[i].name);
        indexjson=strlen(json);
        strcpy(&json[indexjson],"\":");
        indexjson=strlen(json);
        if(array[i].bint!=false){
            char intvaluestr[10];
            sprintf(intvaluestr, "%d",array[i].intvalue);
            strcpy(&json[indexjson],intvaluestr);

        }else{
            strcpy(&json[indexjson],"\"");
            indexjson=strlen(json);
            strcpy(&json[indexjson],array[i].strvalue);
            indexjson=strlen(json);
            strcpy(&json[indexjson],"\"");
        }
        if(i!=json_c->jsonitem.realsize-1){
            indexjson=strlen(json);
            strcpy(&json[indexjson],", ");
        }
    }
    indexjson=strlen(json);
    strcpy(&json[indexjson],"}");
    indexjson=strlen(json);
    json[allsize]='\0';
    return json;
}
char* sv_json_construct_getstring_SEND(sv_json_construct_t* json_c,int* size){
    int allsize=2;
    
    sv_json_item_t* array=json_c->jsonitem.data;
    
    for(int i=0;i<json_c->jsonitem.realsize;i++){
        allsize+=5+strlen(array[i].name);//5=|"":, |
        if(array[i].bint!=false){
            char intvaluestr[10];
            sprintf(intvaluestr, "%d",array[i].intvalue);
            allsize+=strlen(intvaluestr);
        }else{
            allsize+=2+strlen(array[i].strvalue);//2=|""|
        }
    }
    allsize-=2;//last |, |
    char* json=malloc(allsize+1+4);
    int32_t s=allsize+1;
	memcpy(json,&s,4);
    int indexjson=4;
    json[4]='\0';
    strcpy(&json[indexjson],"{");
    char* t=&json[4];
    for(int i=0;i<json_c->jsonitem.realsize;i++){
        indexjson=strlen(&json[4])+4;
        strcpy(&json[indexjson],"\"");
        indexjson=strlen(&json[4])+4;
        strcpy(&json[indexjson],array[i].name);
        indexjson=strlen(&json[4])+4;
        strcpy(&json[indexjson],"\":");
        indexjson=strlen(&json[4])+4;
        if(array[i].bint!=false){
            char intvaluestr[10];
            sprintf(intvaluestr, "%d",array[i].intvalue);
            strcpy(&json[indexjson],intvaluestr);

        }else{
            strcpy(&json[indexjson],"\"");
            indexjson=strlen(&json[4])+4;
            strcpy(&json[indexjson],array[i].strvalue);
            indexjson=strlen(&json[4])+4;
            strcpy(&json[indexjson],"\"");
        }
        if(i!=json_c->jsonitem.realsize-1){
            indexjson=strlen(&json[4])+4;
            strcpy(&json[indexjson],", ");
        }
    }
    indexjson=strlen(&json[4])+4;
    strcpy(&json[indexjson],"}");
    indexjson=strlen(&json[4])+4;
    *size=allsize+1+4;
    json[allsize+4]='\0';
    return json;
}
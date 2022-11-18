#pragma once
#include"json.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"core.h"
int SearchNameKey(json_value* json,const char* name);
json_object_entry* GetNameKey(json_value* json,const char* name);
bool CheckKeyAndValue(json_value* json,const char* key,json_type type);
typedef struct json_item{
    char name[200];
    char* strvalue;
    int intvalue;
    bool bint;
}json_item_t;
void CreateJson_Item(json_item_t* item);
void json_item_setname(json_item_t* item,const char* name);
void json_item_setintvalue(json_item_t* item,int value);
void json_item_setstrvalue(json_item_t* item,const char* value);
typedef struct json_construct{
    arrayd_t jsonitem;
}json_construct_t;
void CreateJson_Construct(json_construct_t* json);
char* json_construct_getstring(json_construct_t* json_c);
void json_construct_addelement(json_construct_t* json,json_item_t item);
char* json_construct_getstring(json_construct_t* json_c);
char* json_construct_getstring_SEND(json_construct_t* json_c,int* size);
void DestroyJson_Construct(json_construct_t* json);
#pragma once
#include"json.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"core.h"
int sv_SearchNameKey(json_value* json,const char* name);
json_object_entry* sv_GetNameKey(json_value* json,const char* name);
bool sv_CheckKeyAndValue(json_value* json,const char* key,json_type type);
typedef struct sv_json_item{
    char name[200];
    char* strvalue;
    int intvalue;
    bool bint;
}sv_json_item_t;
void sv_CreateJson_Item(sv_json_item_t* item);
void sv_json_item_setname(sv_json_item_t* item,const char* name);
void sv_json_item_setintvalue(sv_json_item_t* item,int value);
void sv_json_item_setstrvalue(sv_json_item_t* item,const char* value);
typedef struct sv_json_construct{
    sv_arrayd_t jsonitem;
}sv_json_construct_t;
void sv_CreateJson_Construct(sv_json_construct_t* json);
char* sv_json_construct_getstring(sv_json_construct_t* json_c);
void sv_json_construct_addelement(sv_json_construct_t* json,sv_json_item_t item);
char* sv_json_construct_getstring(sv_json_construct_t* json_c);
char* sv_json_construct_getstring_SEND(sv_json_construct_t* json_c,int* size);
void sv_DestroyJson_Construct(sv_json_construct_t* json);
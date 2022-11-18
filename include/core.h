#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include<stdbool.h>
typedef struct sv_v2{
    void* p1;
    void* p2;
}sv_v2_t;

typedef struct sv_user{
    struct pollfd* pollptr;
}sv_user_t;
void sv_CreateUser(sv_user_t* user);

void sv_user_disconnect(sv_user_t* user,void(*ClearUser)(sv_user_t* user));
typedef struct sv_node{
    void* data;
    int sizedata;
    struct node* next;
}sv_node_t;
typedef struct sv_list{
    sv_node_t*start;
    int sizelist;
}sv_list_t;
void sv_CreateList(sv_list_t* list);
void sv_list_addelement(sv_list_t* list,void* data,int sizedata);
sv_node_t* sv_list_getlast(sv_list_t* list);
typedef struct sv_arrayd{
    void* data;
    int sizeelement;
    int realsize;
    int allsize;
}sv_arrayd_t;
void sv_CreateArrayD(sv_arrayd_t* array);
void sv_InitArrayd(sv_arrayd_t* array,int sizearray,int sizeelement);
void sv_arrayd_addelement(sv_arrayd_t* array,void* element);
void sv_DestroyArrayD(sv_arrayd_t* array);
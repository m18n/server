#pragma once
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include<stdbool.h>
typedef struct v2{
    void* p1;
    void* p2;
}v2_t;

typedef struct user{
    struct pollfd* pollptr;
}user_t;
void CreateUser(user_t* user);

void user_disconnect(user_t* user,void(*ClearUser)(user_t* user));
typedef struct node{
    void* data;
    int sizedata;
    struct node* next;
}node_t;
typedef struct list{
    node_t*start;
    int sizelist;
}list_t;
void CreateList(list_t* list);
void list_addelement(list_t* list,void* data,int sizedata);
node_t* list_getlast(list_t* list);
typedef struct arrayd{
    void* data;
    int sizeelement;
    int realsize;
    int allsize;
}arrayd_t;
void CreateArrayD(arrayd_t* array);
void InitArrayd(arrayd_t* array,int sizearray,int sizeelement);
void arrayd_addelement(arrayd_t* array,void* element);
void DestroyArrayD(arrayd_t* array);
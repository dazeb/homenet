#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "utils.h"


int max(int x, int y)
{
    if (x > y)
        return x;
    else
        return y;
}

int str_len(char* str){
    return strlen(str);
}

int str_compare(int n,char* s1, int start1, char* s2, int start2){
    for(int i=0;i<n;i++){
        if(*(s1+start1+i) != *(s2+start2+i)){
            return 0;
        }
    }
    return 1;
}

int str_contains(char* str, char testStr[]){
    int testLen=str_len(testStr);
    for(int i=0;i<str_len(str);i++){
        if(str_compare(testLen,str,i,testStr,0)){
            return 1;
        }
    }
    return 0;
}

int str_findIndex(char* str, char c){
char *e;
e = strchr(str, c);
if(e==NULL){
    return -1;
}
return (int)(e - str);
}

char* str_substring(char* s,char* str,int start, int end){
int len=str_len(str);
//printf("len: %d \n",len);
if(end==-1){
end=len-1;
}
int size=end-start+1;
memcpy(s,str+start,end-start+1);
s[size]='\0';
return s;
}

void str_set(char* arr, char string[]){
    strcpy(arr, string);
}

int str_isEqual(char* a, char* b){
    return strcmp(a, b)==0;
}

char* str_copy(char* to, char* from){
    strcpy(to, from);
return to;
}

char* str_toLower(char* str){
    for(int i = 0; str[i]; i++){
  str[i] = tolower(str[i]);
}
return str;
}

char* str_toUpper(char* str){
    for(int i = 0; str[i]; i++){
  str[i] = toupper(str[i]);
}
return str;
}

char* str_strip(char* str){
  int end=strlen(str) - 1;
  int start=0;
  // Trim leading space
  while(start<=end&&isspace((unsigned char)str[start])) start++;
  if(str[start] == '\0'){
      str[0]='\0';
  return str;
  }  
  // Trim trailing space
  while(end > start && isspace((unsigned char)str[end])) end--;
  int size=end+1-start;
  for(int i=0;i<size;i++){
      str[i]=str[start+i];
  }
  str[size]='\0';
  return str;
}

char* str_concat(char* str, char* str2){
    // Use strcat directly to take multiple inputs
    strcat(str,str2);
return str;
}

char* str_split(char* str, char delimiter[]){
   return strtok(str, delimiter);
}

int str_toInt(char* str){
    return atoi(str);
}

float str_toFloat(char* str){
    return strtof(str, NULL);
}

char* int_toString(char* out, int i){
sprintf( out, "%d", i );
return out;
}

char* float_toString(char* out, float f){
    sprintf( out, "%f", f );
    return out;
}

int int_charsCount(int i){
    return snprintf( NULL, 0, "%d", i );
}

int float_charsCount(float f){
    return snprintf( NULL, 0, "%f", f );
}

////////////////////

void map_init(Map* map){
    map->start=NULL;
    map->count=0;
}

Item* map_getItem(Map* map,char key[]){
    if(!(map->start)){
    return NULL;
    }
    for(Item* i=map->start;i;i=i->next){
        if(str_isEqual(i->key,key)){
            return i;
        }
    }
    return NULL;
}

Item* map_lastItem(Map* map){
    Item* i=map->start;
    while(i->next){
        i=i->next;
    }
    return i;
}

void map_set(Map* map, char key[], void* value){
    Item* prevItem=map_getItem(map,key);
    
    if(prevItem){
        free(prevItem->value);
        prevItem->value=value;
    }
    else{
        Item* newItem=(Item*)malloc(sizeof(Item));
    str_set(newItem->key,key);
    newItem->value=value;
    newItem->next=map->start;
    map->start=newItem;
    if(map->count)
    map->count++;
    else
    map->count=1;
    }
}

void* map_get(Map* map, char key[]){
    Item* item=map_getItem(map,key);
    if(item){
        return item->value;
    }
    else
    return NULL;
}

int map_del(Map* map, char key[]){
    Item* prev=NULL;
    for(Item* i=map->start;i->next;i=i->next){
        if(str_isEqual(i->key,key)){
            if(prev)
            prev->next=i->next;
            else
            map->start=i->next;
            free(i);
            return 1;
        }
        prev=i;
    }
    return 0;
}

Item* map_forEach(Map* map){
    static Item* item=NULL;
    if(map){
        item=map->start;
        return item;
    }
    if(item)
    item=item->next;
    return item;
}

////////////////////////////////

void list_add(List* store, void* data){
    ListNode* node=(ListNode*)malloc(sizeof(ListNode));
    node->data=data;
    node->next=NULL;
if(store->start){
    node->next=store->start;
}
store->start=node;
}

void list_remove(List* store, void* data){
    ListNode* prev=NULL;
    for(ListNode* i=store->start;i->next;i=i->next){
        if(i->data==data){
            if(prev)
            prev->next=i->next;
            else
            store->start=i->next;
            free(i);
            return 1;
        }
        prev=i;
    }
    return 0;
}

void* list_forEach(List* store){
    static ListNode* node=NULL;
    if(store){
        node=store->start;
        return node->data;
    }
    if(node)
    node=node->next;
    return node->data;
}

//////////////////////

void buffer_init(Buffer* buffer, int capacity){
    if(capacity<=0){
        capacity=DEFAULT_BUFFER_SIZE;
    }
    buffer->data=(char*)malloc(sizeof(char)*capacity);
    buffer->size=0;
    buffer->capacity=capacity;
    buffer->startIndex=0;
}

int buffer_reset(Buffer* buffer){
    free(buffer->data);
    buffer->data=NULL;
    buffer->size=0;
    buffer->capacity=0;
    buffer->startIndex=0;
    return 1;
}

int buffer_getFreeSpace(Buffer* buffer){
    return buffer->capacity-buffer->size;
}

void buffer_print(Buffer* buffer){
    printf("<Buffer[%d]>\n Free space: %d, Capacity: %d, Current start: %d \n [%s]",buffer->size, buffer_getFreeSpace(buffer) ,buffer->capacity,buffer->startIndex,buffer->data);
}

int buffer_write(Buffer* buffer, char* data, int size){
    int freeSpace=buffer_getFreeSpace(buffer);
    if(freeSpace<size){
        size=freeSpace;
    }
     int ind=buffer->startIndex;
    for(int i=0;i<size;i++){
        buffer->data[ind]=data[i];
        ind++;
        if(ind>=buffer->capacity){
            ind=0;
        }
    }
    buffer->size+=size;
    return size;
}

int buffer_read(Buffer* buffer, char* data, int size){
    if(buffer->size==0 || size==0){
        return 0;
    }
    if(buffer->size<size){
        size=buffer->size;
    }
    int ind=buffer->startIndex;
    for(int i=0;i<size;i++){
        data[i]=buffer->data[ind];
        ind++;
        if(ind>=buffer->capacity){
            ind=0;
        }
    }
    buffer->size-=size;
    buffer->startIndex+=size;
    if(buffer->startIndex>=buffer->capacity){
        buffer->startIndex=buffer->capacity-buffer->startIndex;
    }
    return size;
}
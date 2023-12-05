#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct node {
	char key[100];
	char* value;
	struct node* next;
	// UT_hash_handle hh;
};	
typedef struct node node_t;


struct kvs{
	struct node* db; // database
	int items; // number of data 
}; 
typedef struct kvs kvs_t; 

extern int is_recovery;


kvs_t* kvs_open();
int kvs_close(kvs_t* kvs); // free all memory space 
int set(kvs_t* kvs, const char* key, const char* value); // return -1 if failed.
char* get(kvs_t* kvs, const char* key); // return NULL if not found. 

extern int is_recovery;

//baseline 버전 snapshot,recovery
void do_snapshot(kvs_t* kvs);
void do_recovery(kvs_t* kvs);
//custom 버전 snapshot, recovery
void do_cust_snpshot(kvs_t* kvs);
void do_cust_recovery(kvs_t* kvs);

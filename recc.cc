#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string.h>
#include <pthread.h>

#include "sock.cc"

using namespace std;

int sock1;

typedef struct files{
	int id;
	char name[100];
	int size;
	struct files *next;
}files ;

typedef struct dirs{
	int id;
	char name[100];
	struct dirs *next;
}dirs ;


files *f = NULL;
files *st = NULL;

dirs *d = NULL;
dirs *st2 = NULL;

int c_f = 0;
int c_d = 0;

void add_f(char *name, int size){
	files *temp = (files *)malloc(sizeof(files));
	strcpy(temp -> name, name);
	temp -> id = c_f;
	temp -> size = size;
	temp -> next = NULL;
	if(f == NULL){
		f = temp;
		st = f;
	}else{ 
		f -> next = temp;
		f = f -> next;
	}
	c_f++;
}

void add_d(char *name){
	dirs *temp = (dirs *)malloc(sizeof(dirs));
	strcpy(temp -> name, name);
	temp -> id = c_d;
	temp -> next = NULL;
	if(d == NULL){
		d = temp;
		st2 = d;
	}else{ 
		d -> next = temp;
		d = d -> next;
	}
	c_d++;
}

void get_files(){
	char sb[4];
	
	rec(sock1, sb, 4);
	int size = *((int *)sb);

	while (size > 0) {
		char *buffer = (char *)malloc(sizeof(files));
		int cc = rec(sock1, buffer, sizeof(files));
		files *f = (files *)buffer;
		
		add_f(f -> name, f -> size);
		free(buffer);
		size -= cc;
		if(cc < 112){
			char buf[112 - cc];
			cc = rec(sock1, buf, 112 - cc);
			size -= cc;
		}
	}
}
void get_dirs(){
	char sb[4];
	rec(sock1, sb, 4);
	int size = *((int *)sb);
	cout << size << endl;
	
	while (size > 0) {
	
		char *buffer = (char *)malloc(sizeof(dirs));
		int cc = rec(sock1, buffer, sizeof(dirs));
		dirs *d = (dirs *)buffer;
		add_d(d -> name);
		cout << d -> name << endl;
		
		size -= cc;
		if(cc < sizeof(dirs)){
			cc = rec(sock1, buffer, sizeof(dirs) - cc);
			size -= cc;
		}
		free(buffer);
	}
}
void send_msg(int m){
	char msg[4];
	int *s = (int *)msg;
	*s = m;
	sendd(sock1, msg, 4);
}

void re_f(int sock, char *name){
	ofstream w;
	w.open(name, ios::out | ios::binary);
	char sb[4];
	rec(sock, sb, 4);
	int size = *((int *)sb);
	char buffer[512];
	if (w.is_open()) {
		while (size > 0) {
			int cc = rec(sock, buffer, 512);
			w.write(buffer, cc);
			size -= cc;
		}
	}
	close(sock);
	w.close();
}
char *check(int id){
	files *p = st;
	while(p != NULL){
		if(p -> id == id) return p -> name;
		p = p -> next;
	}
	return NULL;
}
void *rec_file(void *arg){
	int new1 = create_sock(4999);
	re_f(new1, (char *)arg);
	close(new1);
	return NULL;
}
void reset(){
	files *p = st;
	while(p != NULL){
		files *q = p;
		p = p -> next;
		free(q);
	}
	dirs *p2 = st2;
	while(p2 != NULL){
		dirs *q2 = p2;
		p2 = p2 -> next;
		free(q2);
	}
	f = NULL;
	d = NULL;
	st = NULL;
	st2 = NULL;
	c_f = c_d = 0;
}
void cd(int num){
	send_msg(num);
	reset();
}
void download(int num){
	send_msg(num);
	pthread_t pth2;
	char *c2 = check(num);
	
	if(c2 != NULL){
		pthread_create(&pth2, NULL, rec_file, (void *)c2);
		pthread_join( pth2, NULL);
	}
}
files *re_st(){
	send_msg(-2);
	get_files();
	return st;
}
dirs *re_st2(){
	send_msg(-1);
	get_dirs();
	return st2;
}
void init(char *r_ip, char *l_ip){
	sock1 = connect_sock(r_ip, "3999");
	if(sock1 == 0) exit(0);
	sendd(sock1, l_ip, 16);
}

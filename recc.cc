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
#include <gtkmm.h>

#define  PORT_COM       3999
#define  PORT_FILE      4999
#define  PORT_COM_STR  "3999"

#include "sock.cc"

using namespace std;

int sock1;
void (*pf)(double t, bool done);
int Size;
int S;

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
files *st_file = NULL;

dirs *d = NULL;
dirs *st_dir = NULL;

int c_f = 0;
int c_d = 0;
bool stop = false;

void add_to_file_queue(char *name, int size){
	files *temp = (files *)malloc(sizeof(files));
	strcpy(temp -> name, name);
	temp -> id = c_f;
	temp -> size = size;
	temp -> next = NULL;
	if(f == NULL){
		f = temp;
		st_file = f;
	}else{ 
		f -> next = temp;
		f = f -> next;
	}
	c_f++;
}

void add_to_dir_queue(char *name){
	dirs *temp = (dirs *)malloc(sizeof(dirs));
	strcpy(temp -> name, name);
	temp -> id = c_d;
	temp -> next = NULL;
	if(d == NULL){
		d = temp;
		st_dir = d;
	}else{ 
		d -> next = temp;
		d = d -> next;
	}
	c_d++;
}

void get_files_list(){
	char sb[4];
	
	rec(sock1, sb, 4);
	int size = *((int *)sb);

	while (size > 0) {
		char *buffer = (char *)malloc(sizeof(files));
		int cc = rec(sock1, buffer, sizeof(files));
		files *f = (files *)buffer;
		
		add_to_file_queue(f -> name, f -> size);
		free(buffer);
		size -= cc;
		if(cc < 112){
			char buf[112 - cc];
			cc = rec(sock1, buf, 112 - cc);
			size -= cc;
			
		}
	}
}
void get_dirs_list(){
	char sb[4];
	rec(sock1, sb, 4);
	int size = *((int *)sb);
	cout << size << endl;
	
	while (size > 0) {
	
		char *buffer = (char *)malloc(sizeof(dirs));
		int cc = rec(sock1, buffer, sizeof(dirs));
		dirs *d = (dirs *)buffer;
		add_to_dir_queue(d -> name);
		cout << d -> name << endl;
		
		size -= cc;
		if(cc < (int)sizeof(dirs)){
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
void *update(void *arg){
	do{
		pf((1.0*(S - Size))/(double)S, false);
		sleep(1);
	}while(Size > 512);
	return NULL;
}
void recv_f(int sock, char *name){
	ofstream w;
	pthread_t pth2;
	w.open(name, ios::out | ios::binary);
	char sb[4];
	rec(sock, sb, 4);
	Size = *((int *)sb);
	char buffer[512];
	S = Size;
	char m[0];
	pthread_create(&pth2, NULL, update, (void *)m);
	if (w.is_open()) {
		while (Size > 0) {
			int cc = rec(sock, buffer, 512);
			w.write(buffer, cc);
			Size -= cc;

			if(stop){
				pthread_cancel(pth2);
				pf(0.0, true);
				sleep(1);
				close(sock);
				w.close();
				return;
			}
		}
	}
	Size = 0;
	close(sock);
	w.close();
	pf(1.0, true);
}
char *check(int id){
	files *p = st_file;
	while(p != NULL){
		if(p -> id == id) return p -> name;
		p = p -> next;
	}
	return NULL;
}
void *recv_file(void *arg){
	int new1 = create_sock(PORT_FILE);
	recv_f(new1, (char *)arg);
	close(new1);
	return NULL;
}
void reset(){
	/*
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
	}*/
	f = NULL;
	d = NULL;
	st_file = NULL;
	st_dir = NULL;
	c_f = c_d = 0;
}
void cd(int num){
	send_msg(num);
	reset();
}
void download(string dir, int num, void (*progress_func)(double t, bool done)){
	send_msg(num);
	pthread_t pth2;
	char *c2 = check(num);
	pf = progress_func;
	cout << "cc " << num << endl;
	const char *dire = dir.c_str();
	
	char *name = (char *)malloc(strlen(dire) + strlen(c2) + 2);
	
	strcpy(name, dire);
	strcat(name, "/");
	strcat(name, c2);
	
	stop = false;
	if(c2 != NULL){
		pthread_create(&pth2, NULL, recv_file, (void *)name);
	}
}
files *refresh_files_queue(){
	send_msg(-2);
	get_files_list();
	return st_file;
}
dirs *refresh_dirs_queue(){
	send_msg(-1);
	get_dirs_list();
	return st_dir;
}
void close_sock(){
	if(sock1 == 0) close(sock1);
}
void set_stop(){
	stop = true;
}
int init(char *r_ip, char *l_ip){
	sock1 = connect_sock(r_ip, PORT_COM_STR);
	if(sock1 == 0) return 0;
	sendd(sock1, l_ip, 16);
	return 1;
}

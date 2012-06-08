#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>

#include "sock.cc"


using namespace std;

char *ip;
int new1;

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
	if(strlen(name) > 99){
		name[99] = '\0';
	}
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

void add_f_d(char *name){
	dirs *temp = (dirs *)malloc(sizeof(dirs));
	if(strlen(name) > 99){
		name[99] = '\0';
	}
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

void read_files(){
	DIR *dir;
	struct dirent *ent;
	dir = opendir(".");
	
	if(dir != NULL){
		while((ent = readdir(dir)) != NULL){
			struct stat st_buf;
			stat(ent -> d_name, &st_buf);
			if(S_ISREG(st_buf.st_mode)){
				add_f(ent -> d_name, st_buf.st_size);
			}
			if(S_ISDIR(st_buf.st_mode)){
				add_f_d(ent -> d_name);
			}
			
		}
		closedir(dir);
	}
}
void send_names(int sock){
	files *p = st;
	char s_buf[4];
	int *size = (int *)s_buf;
	*size = c_f * sizeof(files);
	sendd(new1, s_buf, 4);
	while(p != NULL){
		sendd(new1, (char *)p, sizeof(files));
		p = p -> next;
	}
}
void copy_file(int sock, char *fi){
	char pwd[100];
	getcwd(pwd, 100);

	ifstream r;
	r.open(fi, ios::in | ios::binary);
	
	struct stat statbuf;

	if (stat(fi, &statbuf) == -1) {
		close(sock);
		return;
	}

	char s_buf[4];
	int* size = (int *)s_buf;
	*size = statbuf.st_size;
	r.seekg(0, std::ios::beg);
	sendd(sock, s_buf, 4);
	char buffer[512];
	if (r.is_open()) {
		while (*size > 0) {
			if(*size < 512){
				r.read(buffer, *size);
				if(sendd(sock, buffer, *size) == -1){
					r.close();
					close(sock);
					sleep(1);
					return;
				}
			}else{
				r.read(buffer, 512);
				if(sendd(sock, buffer, 512) == -1){
					r.close();
					close(sock);
					sleep(1);
					return;
				}
			}
			*size -= 512;
		}
	}
	r.close();
}
void *send_file(void *arg){
	sleep(0.5);
	int sock = connect_sock(ip, "4999");
	if(sock == 0){
		sleep(1);
		cout << "Try again !!!" << endl;
		sock = connect_sock(ip, "4999");
		//return NULL;
	}
	if(sock == 0){
		return NULL;
	}
	char *fil = (char *)arg;
	copy_file(sock, fil);
	close(sock);
	return NULL;
}
void send_dirs(int sock){
	char s_buf[4];
	int *size = (int *)s_buf;
	*size = c_d * sizeof(dirs);
	sendd(new1, s_buf, 4);
	dirs *p2 = st2;
	while(p2 != NULL){
		sendd(new1, (char *)p2, sizeof(dirs));
		p2 = p2 -> next;
	}
}
char *check(int id){
	files *p = st;
	while(p != NULL){
		if(p -> id == id) return p -> name;
		p = p -> next;
	}
	return NULL;
}
char *check2(int id){
	dirs *p = st2;
	while(p != NULL){
		if(p -> id == id) return p -> name;
		p = p -> next;
	}
	return NULL;
}
void reset(){
	f = NULL;
	d = NULL;
	st = NULL;
	st2 = NULL;
	c_f = c_d = 0;
}
int main(){
	chdir("/home/omega/dwhelper");
	new1 = create_sock(3999);
	if(new1 == 0) return 1;
	char msg[16];
	rec(new1, msg, 16);
	ip = msg;

	read_files();
	while(1){
		char msg[4];
		int *size = ((int *)msg);
		rec(new1, msg, 4);

		if(*size == -1)
			send_dirs(new1);
		else if(*size == -2)
			send_names(new1);
		else if(*size == -3){
			close(new1);
			exit(0);
		}else if(*size < -3){

			int x = *size;
			x = -1*x - 4;
			char *c = check2(x);
			if(c != NULL){
				chdir(c);
				reset();
				read_files();
			}
		}else if(*size >= 0){
			pthread_t pth;
			char *c = check(*size);
			if(c != NULL){
				pthread_create(&pth, NULL, send_file, (void *)c);
				pthread_join( pth, NULL);
			}
		}
	}
	return 0;
}

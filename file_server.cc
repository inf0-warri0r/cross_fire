/*
*Author :Tharindra Galahena
*Project:cross fire - network file sharing application for linux
*Date   :20/06/2012
*/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

#define  PORT_COM       3999
#define  PORT_FILE      4999
#define  PORT_FILE_STR "4999"

#include "sock.h"
#include "file_dir_struct.h"

using namespace std;

char *ip;
int clint_sock;

files *f = NULL;
files *st_file = NULL;

dirs *d = NULL;
dirs *st_dir = NULL;

int c_f = 0;
int c_d = 0;

void headder(){
	cout << "-inf0_warri0r-" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << " ______   ______   ______   _____   _____  " << endl;
	cout << "|  ____| |  __  | |  __  | |  ___| |  ___| " << endl;
	cout << "| |      | |__| | | |  | | | |___  | |___  " << endl;
	cout << "| |      |  __  / | |  | | |___  | |___  | " << endl;
	cout << "| |____  | | \\ \\  | |__| |  ___| |  ___| | " << endl;
	cout << "|______| |_|  \\_\\ |______| |_____| |_____| " << endl;
	
	cout << "				 ______   _____   ______   ______   " << endl;
	cout << "				|  ____| |_   _| |  __  | |  ____|  " << endl;
	cout << "				| |___     | |   | |__| | | |__     " << endl;
	cout << "				|  ___|    | |   |  __  / |  __|    " << endl;
	cout << "				| |       _| |_  | | \\ \\  | |____   " << endl;
	cout << "				|_|      |_____| |_|  \\_\\ |______|  " << endl;
	cout << "---------------------------------------------------\n\n" << endl;
	
	
	cout << "file server" << endl;
}
void add_file(char *name, int size){
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
		st_file = f;
	}else{ 
		f -> next = temp;
		f = f -> next;
	}
	c_f++;
}

void add_dir(char *name){
	dirs *temp = (dirs *)malloc(sizeof(dirs));
	if(strlen(name) > 99){
		name[99] = '\0';
	}
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

void read_files(){
	DIR *dir;
	struct dirent *ent;
	dir = opendir(".");
	
	if(dir != NULL){
		while((ent = readdir(dir)) != NULL){
			struct stat st_buf;
			stat(ent -> d_name, &st_buf);
			if(S_ISREG(st_buf.st_mode)){
				add_file(ent -> d_name, st_buf.st_size);
			}
			if(S_ISDIR(st_buf.st_mode)){
				add_dir(ent -> d_name);
			}
			
		}
		closedir(dir);
	}
}
int send_file_names(int sock){
	files *p = st_file;
	char s_buf[4];
	int *size = (int *)s_buf;
	*size = c_f * sizeof(files);
	if(sendd(clint_sock, s_buf, 4) == -1) return -1;
	while(p != NULL){
		if(sendd(clint_sock, (char *)p, sizeof(files)) == -1) return -1;
		p = p -> next;
	}
	return 0;
}
int send_dir_names(int sock){
	char s_buf[4];
	int *size = (int *)s_buf;
	*size = c_d * sizeof(dirs);
	if(sendd(clint_sock, s_buf, 4) == -1) return -1;
	dirs *p2 = st_dir;
	while(p2 != NULL){
		if(sendd(clint_sock, (char *)p2, sizeof(dirs)) == -1) return -1; 
		p2 = p2 -> next;
	}
	return 0;
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
	if(sendd(sock, s_buf, 4) == -1) return;
	char buffer[512];
	if (r.is_open()) {
		while (*size > 0) {
			if(*size < 512){
				r.read(buffer, *size);
				if(sendd(sock, buffer, *size) == -1){
					r.close();
					close(sock);
					sleep(1);
					cout << "\n--interripeted--\n" << endl;
					return;
				}
			}else{
				r.read(buffer, 512);
				if(sendd(sock, buffer, 512) == -1){
					r.close();
					close(sock);
					sleep(1);
					cout << "\n--interripeted--\n" << endl;
					return;
				}
			}
			*size -= 512;
		}
	}
	cout << "\n--file sent--\n" << endl;
	r.close();
}
void *send_file(void *arg){
	sleep(0.5);
	
	cout << "\nfirst attempt to connect to file port : " << endl;
	int sock = connect_sock(ip, PORT_FILE_STR);
	if(sock == 0){
		sleep(1);
		cout << "\nsecond attempt to connect to file port : " << endl;
		sock = connect_sock(ip, PORT_FILE_STR);
	}
	if(sock == 0){
		return NULL;
	}
	cout << "\n--connected-- " << endl;
	char *fil = (char *)arg;
	
	cout << "\nSending file : " << fil << endl;
	
	copy_file(sock, fil);
	close(sock);
	return NULL;
}

char *check_file(int id){
	files *p = st_file;
	while(p != NULL){
		if(p -> id == id) return p -> name;
		p = p -> next;
	}
	return NULL;
}
char *check_file_dir(int id){
	dirs *p = st_dir;
	while(p != NULL){
		if(p -> id == id) return p -> name;
		p = p -> next;
	}
	return NULL;
}
void reset(){
	f = NULL;
	d = NULL;
	st_file = NULL;
	st_dir = NULL;
	c_f = c_d = 0;
}
void refresh(){
	close(clint_sock);
	sleep(0.5);
	reset();
	cout << "\n--clint disconnected--\n" << endl;
}
int main(){
	
	headder();
	start:
	cout << "--------------------------" << endl;
	cout << "witing for clint :" << endl;
	cout << "--------------------------\n" << endl;
	chdir(getenv("HOME"));
	cout << "PWD : " << getenv("PWD") << endl;
	
	clint_sock = create_sock(PORT_COM);
	if(clint_sock == 0) return 1;
	cout << "\n--clint connected--\n" << endl;
	ip = (char *)malloc(16);
	rec(clint_sock, ip, 16);

	read_files();
	while(1){
		char msg[4];
		int *commend_num = ((int *)msg);
			if(rec(clint_sock, msg, 4) == -1){
				refresh();
				goto start;
			}

		if(*commend_num == -1){
			if(send_dir_names(clint_sock) == -1){
				refresh();
				goto start;
			}
		}else if(*commend_num == -2){
			if(send_file_names(clint_sock) == -1){
				refresh();
				goto start;
			}
		}else if(*commend_num == -3){
			refresh();
			goto start;
		}else if(*commend_num < -3){
			int x = *commend_num;
			x = -1*x - 4;
			char *c = check_file_dir(x);
			if(c != NULL){
				chdir(c);
				reset();
				char *cwd = (char *)malloc(1024);
				cout << "PWD : " << getcwd(cwd, 1024) << endl;
				free(cwd);
				read_files();
			}
		}else if(*commend_num >= 0){
			pthread_t pth;
			char *c = check_file(*commend_num);
			if(c != NULL){
				pthread_create(&pth, NULL, send_file, (void *)c);
				pthread_join( pth, NULL);
			}
		}
	}
	return 0;
}

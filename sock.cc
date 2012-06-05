#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <netdb.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

char *r_ip;
struct sockaddr_in ad, add;

int create_sock(int port){
	int sock;
	if(!(sock = socket(AF_INET, SOCK_STREAM, 0))){
		printf("ERROR : error creating socket\n");
	}
	
	ad.sin_family = AF_INET;
	ad.sin_addr.s_addr = INADDR_ANY;
	ad.sin_port = htons(port);

	bind(sock, (struct sockaddr *)&ad, sizeof(ad));
	listen(sock, 1);

	socklen_t addrlen = sizeof(add);
	int new1 = accept(sock, (struct sockaddr *)&add, &addrlen);
	
    
	if(new1) printf("Connected to player 1\n");
	else {
		printf("ERROR : error connecting\n");
		return 0;	
	}
	if(close(sock) == -1) perror("ERROR!\n");
	return new1;
}
void get_ip(char *buf){
	inet_ntop(AF_INET, (struct sockaddr *)&add, buf, INET_ADDRSTRLEN);
    printf("his address = %s\n", buf);
}
void sendd(int new1, char *buff, int n){
	if(send(new1, buff, n, 0) == -1){
		perror("ERROR!\n");
		close(new1);
		exit(0);	
	}
}
int connect_sock(char *ip, char *port){
	struct addrinfo hints, *res;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(ip, port, &hints, &res);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(sock, res -> ai_addr, res -> ai_addrlen) == -1){
		perror("error\n");
		return 0;	
	}
	return sock;
}
int rec(int sock1, char *msg, int n){
	int cc;
	cc = recv(sock1, msg, n, 0);
	if(cc == -1){
		perror("ERROR");
		close(sock1);
	}
	return cc;
}

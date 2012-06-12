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

int create_sock(int port){
	int sock;
	int yes = 1;
	struct sockaddr_in ad, add;
	if(!(sock = socket(AF_INET, SOCK_STREAM, 0))){
		printf("ERROR : error creating socket\n");
	}
	
	ad.sin_family = AF_INET;
	ad.sin_addr.s_addr = INADDR_ANY;
	ad.sin_port = htons(port);

	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
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

int sendd(int new1, char *buff, int n){
	if(send(new1, buff, n, 0) == -1){
		perror("ERROR!\n");
		close(new1);
		return -1;	
	}
	return 0;
}
int connect_sock(char *ip, char *port){
	struct addrinfo hints, *res, *p;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int sock;
	getaddrinfo(ip, port, &hints, &res);
	for(p = res; p != NULL; p = p->ai_next) {
	
		if((sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("error\n");
			continue;
		}
		if(connect(sock, p -> ai_addr, p -> ai_addrlen) == -1){
			perror("error\n");
			continue;	
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
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

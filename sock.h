/*
*Author :Tharindra Galahena
*Project:cross fire - network file sharing application for linux
*Date   :20/06/2012
*/

#include "sock.cc"

int create_sock(int port);
int sendd(int new1, char *buff, int n);
int connect_sock(char *ip, char *port);
int rec(int sock1, char *msg, int n);

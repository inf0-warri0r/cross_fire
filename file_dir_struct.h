/*
*Author :Tharindra Galahena
*Project:cross fire - network file sharing application for linux
*Date   :20/06/2012
*/

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

#Author :Tharindra Galahena
#Project:cross fire - network file sharing application for linux
#Date   :20/06/2012

CC    = g++
FILE1 = client_main.cc
FILE2 = file_server.cc
FLAGS = -lpthread
GTK_LIBS = $(shell pkg-config --libs --cflags gtkmm-2.4)

all:	
	$(CC) -o client_main $(FILE1) $(FLAGS) $(GTK_LIBS)	
	$(CC) -o file_server $(FILE2) $(FLAGS) 

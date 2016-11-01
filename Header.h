#ifndef HEADER_FILE
#define HEADER_FILE
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>  
#include <signal.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <iostream>
using namespace std;

#define BUFLEN 512  //Max length of buffer
 
//extern int filesize;


int die(char const *s);
void cleanExit(int);
int open_send (int s,struct sockaddr_in ToServer, socklen_t slen, char* file, char* buf);
long fsize(const char *filename) ;
int receive_send (int s,struct sockaddr_in ToClient, socklen_t clen, char* file, char* buf,struct sockaddr_in ToServer);
#endif
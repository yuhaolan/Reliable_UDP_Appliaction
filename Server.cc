#include <sys/wait.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>  
#include <signal.h>
#include <iostream>
#include "Header.h"
using namespace std;

 
#define BUFLEN 512  //Max length of buffer
 

 
int main(int argc, char * argv[])
{
    struct sockaddr_in ToClient, ToServer;
     char message[BUFLEN];

    int s,s1;
    socklen_t slen = sizeof(ToServer);
    socklen_t clen = sizeof(ToClient);
    int recv_len;
    char* buf;
    buf = (char*) malloc( (BUFLEN+1)*sizeof(char) );
    pid_t pid;
    if (argc != 3)
    {
        perror("Usage:./Server Name_of_File_To_Send Client_Port ");
        return 0;
    }
    char* file;
    file = argv[1];
    int PORT = atoi(argv[2]);

   label:
     message[0] = '\0';

    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        die("socket");
    }
    //allow new forked process to use the same UDP port
    int enable = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        die("setsockopt(SO_REUSEADDR) failed");  
    // zero out the structure
    memset((char *) &ToClient, 0, sizeof(ToClient));
     
    ToClient.sin_family = AF_INET;
    ToClient.sin_port = htons(PORT);
    ToClient.sin_addr.s_addr = htonl(INADDR_ANY);
    /*
   if (inet_aton(CLIENT_IP , &ToClient.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    */
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&ToClient, sizeof(ToClient) ) == -1)
    {
        die("bind");
    }
    
    //receive(s,ToServer,BUFLEN,buf)

    
    
        

       
        
    printf("Waiting for data...\n");
    fflush(stdout);
        
         
    //try to receive some data, this is a blocking call
        // the requested message
size_t check = 0;

while(1)
{
    message[0] = '\0';
check = recvfrom(s, message, BUFLEN, 0, (struct sockaddr *) &ToServer, &slen);

    if (check > 0)    
    {
        pid = fork();
        if(pid == 0)
        {
            buf[0] = '\0';
            check = recvfrom(s, message, BUFLEN, 0, (struct sockaddr *) &ToServer, &slen);
            //printf("Received packet from %s:%d\n", inet_ntoa(ToServer.sin_addr), ntohs(ToServer.sin_port));
            printf("===>Received requested message successfully!!: %s\n" , message);
            //int open_send(int s,struct sockaddr_in server, int slen, char* file, char* buf)
            open_send (s,ToServer,slen,file,buf);
            if ( close(s) < 0)
            {
                die("Failed to close socket");
            }
            buf[0] = '\0';
            check = 0;
            goto label;

        }
        else if (pid == -1)
        {
            die("frok() failed");
        }
        else
        {
            if ( close(s) < 0)
            {
                die("Failed to close socket");
            }
            wait(NULL);
            check = 0;
            buf[0] = '\0';
            

        }
        

    }
    check = 0;


}
    

/*
       while(1)
       {  
            printf("Enter message : ");
            fgets(buf,BUFLEN,stdin);
            //print details of the ToClient/peer and the data received
       
         //because toserver has the address of the client so sendto Toserver
            if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &ToServer, slen) < 0)
            {
                perror("sendto()");
            }
       
        
        }
*/
   
    if ( close(s) < 0)
    {
        die("Failed to close socket");
    }
    free(buf);
    signal(SIGTERM,cleanExit);
    signal(SIGINT,cleanExit);
    return 0;
}

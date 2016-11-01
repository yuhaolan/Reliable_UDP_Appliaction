
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
#include "Header.h"
using namespace std;
#define BUFLEN 512  //Max length of buffer
 

//ip 127.0.0.1
 
int main(int argc, char * argv[])
{
    struct sockaddr_in ToClient,ToServer;
    int s, i, slen=sizeof(ToServer);
    socklen_t clen = sizeof(ToClient);
    //char message[BUFLEN];


    char* buf;
    buf = (char*) malloc( (BUFLEN+1)*sizeof(char) );
    if (argc != 4)
    {
        perror("Usage:./Client SERVER_IP Dest_Port File_Name_To_Save");
        return 0;
    }

    char* SERVER_IP;
    int PORT;
    char* file;
    SERVER_IP = argv[1];
    PORT = atoi(argv[2]);
    file = argv[3];

    if ( (s=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        die("cannot create socket");
           
    }
 
    memset((char *) &ToServer, 0, sizeof(ToServer));
    ToServer.sin_family = AF_INET;
    ToServer.sin_port = htons(PORT);
     
    if (inet_aton(SERVER_IP , &ToServer.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }


    char const * message = "Please send me a file";

    cout << "=>Send requested message  successfully" << endl;       
    //fgets(message,BUFLEN,stdin);
         
        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &ToServer, slen) < 0)
        {
            die("sendto()");
        }

    receive_send (s,ToClient, clen, file, buf,ToServer);

    /*    
    while(1)
    {
        
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        //memset(buf,'\0', BUFLEN);
        
        //try to receive some data, this is a blocking call
        printf("Waiting for data...");
        fflush(stdout);
        
         
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &ToClient, &clen) );

      
        printf("Received packet from %s:%d\n", inet_ntoa(ToClient.sin_addr), ntohs(ToClient.sin_port));
        printf("Data==>: %s\n" , buf);

        
    
    }
    */
    signal(SIGTERM,cleanExit);
    signal(SIGINT,cleanExit);

    if ( close(s) < 0)
    {
        die("Failed to close socket");
    }
    free(buf);
    return 0;
}

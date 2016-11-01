#include <sys/wait.h>
#include <stdio.h>
#include <sys/stat.h>
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
#include "Header.h"
#include <iostream>
#include <pthread.h>


using namespace std;

#define BUFLEN 512  //Max length of buffer
 

long fsize(const char *filename) 
{
	long size;
	FILE * f = fopen(filename,"rb");
	if (f)
	{
   		fseek(f, 0 ,SEEK_END);
   		size = ftell(f);
   		fclose(f);
	} 
	return size;
}

int die(char const *s)
{
    perror(s);
    return 0;
}

void cleanExit(int)
{
   exit(0);
}



int open_send (int s,struct sockaddr_in ToServer, socklen_t slen, char* file, char* buf)
{
    pthread_mutex_t lock;

    pthread_mutex_lock(&lock);

	FILE* fp; 
     fp = fopen(file,"rb");
     if(fp <= 0)
        die("open a file failed");

        


    // mutex

    long filesize = fsize(file);
    //cout << filesize << endl;
    char rec_check[3];
    long checksize = 0;
    size_t check = 0;
    string str;
    size_t read;
    rewind(fp);
    while(filesize > checksize)
    {
            read = 0;
            memset(rec_check, 0, 3);
            memset(buf, 0, BUFLEN);
            buf[0] = '\0';
            //!!!!!!!!!
            //will the fp mv???
            //text file ok
            //pdf is wrong
            //*
           // while(read == 0 || strlen(buf) == 0)
            //{
                

            read = fread(buf,sizeof(char),BUFLEN,fp);

              //  cout << "print :" <<read << endl;
                //cout << "length :" << strlen(buf) << endl;
               
              //  if (read == 0 || strlen(buf) == 0)
                //{
                    //rewind(fp);
                    //fseek(fp, checksize, SEEK_SET);//check the file pointer
                //}
                

            //}
            

            if (strlen(buf) == 0 || read == 0 )
            {
                die ("aaaaa");
            }
            

            if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &ToServer, slen) < 0)
            {
                die("sendto() failed");
            }
        
            check = recvfrom(s, rec_check, 3, 0, (struct sockaddr *) &ToServer, &slen);
            str = string(rec_check);
            
            if (str == "RS")
            { 
                //cout << "rec_check = " << rec_check << endl;
                if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &ToServer, slen) < 0)
                {
                    die("sendto() failed");
                }
                check = recvfrom(s, rec_check, 3, 0, (struct sockaddr *) &ToServer, &slen);
                str = string(rec_check);                
            }
    /*
            if (str == "RD")
            {
                cout << "rec_check = " << rec_check << endl;
            }
            */
        
        //cout << "check = " << check << endl;

        checksize += read;
        cout << "Complete " << (float) checksize/ (float) filesize * 100<<"%" << endl;

        //cout << "checksize" << checksize << endl;
    }
    cout << "==>Send successfully!!!!!"<<endl;
    char const* message = "ok";

    // send the signal to make client know that the server has finished sending
    if (sendto(s, message, strlen(message), 0, (struct sockaddr*) &ToServer, slen) < 0)
    {
        die("sendto() failed");
    }

    pthread_mutex_unlock(&lock);




    fclose(fp);
    return 0;
}
//    receive_send (s,ToClient, clen, file, buf);

int receive_send (int s,struct sockaddr_in ToClient, socklen_t clen, char* file, char* buf,struct sockaddr_in ToServer)
{
	FILE* fw;
	fw = fopen(file,"wb");
    size_t check;
    int i = 0;
    
    struct timeval timeout;      
    timeout.tv_sec = 4;
    timeout.tv_usec = 0;

    if (setsockopt (s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
            die("setsockopt failed\n");

    if (setsockopt (s, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
            die("setsockopt failed\n");


    while(1)
    {
        memset(buf, 0, BUFLEN);
        buf[0] = '\0';
        check = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &ToClient, &clen);
        //printf("Received packet%d from %s:%d\n",++i, inet_ntoa(ToClient.sin_addr), ntohs(ToClient.sin_port));
        //size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
        //printf("Data==>: %s\n" , buf);
        //cout << "strlen of buf" << strlen(buf) << endl;
        //cout << "check = " << check << "packet " << ++i << endl;
        char const* message = "RD";
        char const* resend = "RS";
        
        while (check == 0)
        {
            //cout << "check = " << check << "packet " << ++i << endl;

            sendto(s, resend, strlen(resend) , 0 , (struct sockaddr *) &ToServer, sizeof(ToServer));
            check = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &ToClient, &clen);
        }
        
        if (check > 0)
        {
            sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &ToServer, sizeof(ToServer));
        }

        string str = string(buf);
        //check whether receive completely
        if (str == "ok")
        {   
            printf("===>Received completely and successfully!!!\n");
            break;
        }
        if (fwrite(buf,sizeof(char),strlen(buf),fw) < 0)
        {
            die("fwrite failed");
        }


    }
    fclose(fw);
    return 0;
}


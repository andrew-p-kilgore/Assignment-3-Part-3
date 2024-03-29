﻿#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 9001 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/

int main (int argc, char **argv)
{
 FILE *fp;
 int listenfd, connfd, n;
 pid_t childpid;
 socklen_t clilen;
 char buf[MAXLINE];
 char newbuf[MAXLINE];
 struct sockaddr_in cliaddr, servaddr;
 int i;
 int j;
 int portnum;
 //Create a socket for the soclet
 //If sockfd<0 there was an error in the creation of the socket
 if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }


 //preparation of the socket address
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons((int) strtol(argv[1], (char **)NULL, 10));

 //bind the socket
 bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

 //listen to the socket by creating a connection queue, then wait for clients
 listen (listenfd, LISTENQ);

 printf("%s\n","Server running...waiting for connections.");

 for ( ; ; ) {

  clilen = sizeof(cliaddr);
  //accept a connection
  connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);

  printf("%s\n","Received request...");

  if ( (childpid = fork ()) == 0 ) {//if it’s 0, it’s child process

    printf ("%s\n","Child created for dealing with client requests");
    
    printf("Server worker PID: %d\n",getpid());
    system("date");
    //close listening socket
    close (listenfd);
    //while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
    for(i=0;i<8;i++){
    memset(buf,'\0',MAXLINE); 
    recv(connfd, buf, MAXLINE, 0);
    printf("From Client: %s",buf);
    if(strcmp(buf,"end\n") == 0) {
    	printf("Server worker terminated\n");
	strcpy(buf,"Terminating connection...\n");
	send(connfd, buf, strlen(buf), 0);
	close(connfd);
	pclose(fp);
	exit(0);
    }
    fp = popen(buf,"r");
    
    while(fgets(buf,MAXLINE,fp) != NULL) 
    	send(connfd, buf, strlen(buf), 0);

    memset(buf,'\0',MAXLINE);
    }

    //if (n < 0)
    //  printf("%s\n", "Read error");
    exit(0);
  }
  //close socket of the server
  close(connfd);
  exit(0);
 }
}

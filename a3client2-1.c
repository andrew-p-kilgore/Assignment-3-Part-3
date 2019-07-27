#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

int
main(int argc, char **argv) 
{
int sockfd;
struct sockaddr_in servaddr;
char sendline[MAXLINE], recvline[MAXLINE];
char filename[MAXLINE];
char comstr[MAXLINE];
FILE *fp;
FILE *fp2;
char endstr[MAXLINE];
strcpy(endstr,"end\n");
char nwln[4];
strcpy(nwln,"\n");
int i = 0;
int j = 0;
int n = 0;
//basic check of the arguments
//additional checks can be inserted
//there should be 5 arguments including "./a3client"
if (argc !=5) {
 	perror("Usage: TCPClient <IP address of the server"); 
	exit(1);
}
	
//Create a socket for the client
//If sockfd<0 there was an error in the creation of the socket
if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
	perror("Problem in creating the socket");
  	exit(2);
}
	
//Creation of the socket
memset(&servaddr, 0, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr= inet_addr(argv[1]);
servaddr.sin_port =  htons((int) strtol(argv[2], (char **)NULL, 10)); //convert to big-endian order

//Connection of the client to the socket 
if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
	perror("Problem in connecting to the server");
	exit(3);
}
	
/* while (fgets(sendline, MAXLINE, stdin) != NULL) {
	
  send(sockfd, sendline, strlen(sendline), 0);
		
  if (recv(sockfd, recvline, MAXLINE,0) == 0){
   //error: server terminated prematurely
   perror("The server terminated prematurely"); 
   exit(4);
  }
  printf("%s", "String received from the server: ");
  fputs(recvline, stdout);
 }*/
 
strcpy(filename,argv[3]); //argv[3] is the one after IP address and Port#
 
fp = fopen(filename,"r");
fp2 = fopen(argv[4],"a"); //argv[4] is the one after the command-file name
if(fp == NULL) {
	perror("Error opening file");
	return(-1);
}
for(i=0;i<8;i++){
	fgets(sendline, MAXLINE, fp);
	printf("Sent to the server: %s",sendline);
	send(sockfd, sendline, strlen(sendline), 0);
	if(strcmp(sendline,endstr) == 0){
		printf("Exiting...\n");
		fclose(fp);
		fclose(fp2);
		exit(0);
	}
	//for(j=0;j<MAXLINE;j++){sendline[j]='\0'; recvline[j]='\0';}
	n = recv(sockfd, recvline, MAXLINE, 0);
	printf("Received from the server: %s",recvline);
	fputs(recvline,fp2);
	if(strcmp(sendline,"ps\n") == 0 || strcmp(sendline,"ls -l | head\n") == 0 ||
	   strcmp(sendline,"ls -l\n") == 0 || strcmp(sendline,"ls | head\n") == 0){
		recv(sockfd, recvline, MAXLINE, 0);
		printf(recvline);
		fputs(recvline,fp2);
	}
	for(j=0;j<MAXLINE;j++){sendline[j]='\0'; recvline[j]='\0';}
	
}
fclose(fp);
exit(0);
}

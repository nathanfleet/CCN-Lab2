// Nathan Fleet
// CCN Lab 2
// 4/28/2023

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 750 // changed for each test
#define PORT 8080
#define SA struct sockaddr
#include <unistd.h>
#include<arpa/inet.h>

void func(int sockfd)
{
    char buff[MAX];
    int n;

    bzero(buff, sizeof(buff));

    // send server file text
    char c;
    FILE *file;
    file = fopen("file.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    } else {
        for (int i = 0; i < MAX; ++i) {
            if ((c = getc(file)) != EOF) {
                buff[i] = c;
            }
        }
        fclose(file);
    }

    // Print the data that is going to be sent to the server
    printf("To server: %s\n", buff);

    write(sockfd, buff, sizeof(buff));
    bzero(buff, sizeof(buff));
    
    read(sockfd, buff, sizeof(buff));
    printf("From Server : %s\n", buff);
}

int main()
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function for chat
	func(sockfd);

	// close the socket
	close(sockfd);
}


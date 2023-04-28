// Nathan Fleet
// CCN Lab 2
// 4/28/2023

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 750 // changed for each test
#define PORT 8080
#define SA struct sockaddr
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>

void func(int sockfd)
{
    char buff[MAX];
    int n;
    struct timeval start, end;
    double total_time;

    gettimeofday(&start, NULL);
    
    // read the message from client and copy it in buffer
    read(sockfd, buff, sizeof(buff));
    printf("From client: %s\n", buff);
    bzero(buff, sizeof(buff));

    gettimeofday(&end, NULL);

    total_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    // Convert total_time to string and store it in buff
    snprintf(buff, MAX, "Total time required to send file from client to server --> %.6f seconds", total_time);

    // Print the data that is going to be sent to the client
    printf("To client: %s\n", buff);

    // Send the total time buffer to the client
    write(sockfd, buff, strlen(buff));
}

// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}


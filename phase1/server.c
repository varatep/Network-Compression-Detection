// Varatep Buranintu 
// Network Compression Detection 
// Server implementation 
// 11/26/2014

// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

int num_packets = 0;
char *host = 0;


void error(const char *msg) {
	perror(msg);
	exit(1);
}

int receive_num_packets(char *argv[]) {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[MAX_BUFFER];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
		fprintf(stderr, "ERR: No port given\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("cant open socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("err on binding");
	}
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		error("error on accept");
	}
	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);
	if (n < 0) {
		error("err reading from socket");
	}
	printf("here is the message: %s\n", buffer);
	n = write(newsockfd, "got message", 18);
	if (n < 0) {
		error("err writing to socket");
	}
	close(newsockfd);
	close(sockfd);

	return 0;
}

int receive_datagram(char *argv[]) {
	int sock, length, n;
	socklen_t fromlen;
	struct sockaddr_in server;
	struct sockaddr_in from;
	char buffer[MAX_BUFFER];

	sock=socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) error("error opening udp socket");
	length = sizeof(server);
	bzero(&server,length);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(argv[1]));
	if (bind(sock,(struct sockaddr *)&server,length)<0) 
		error("binding");
	fromlen = sizeof(struct sockaddr_in);
	while (1) {
		n = recvfrom(sock,buffer,1024,0,(struct sockaddr *)&from,&fromlen);
		if (n < 0) error("recvfrom");
		write(1, "received datagram: ", 19);
		write(1, buf, n);
		n = sendto(sock,"received message\n", 17, 0, (struct sockaddr *)&from, fromlen);
		if (n < 0) {
			error("error with sendto()");
		}
	}
	return 0;	
}

int main(int argc, char *argv[]) {
	if (argc < 1) {
		fprintf(stderr, "usage: ./server port\n");
		exit(0);
	}



}


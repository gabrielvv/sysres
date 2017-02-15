#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h> //write - read

// SEE man 3 getaddrinfo client-server program example
int main(int argc, char* argv){

	setbuf(stdout, NULL);
	int sfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in servaddr;
	bzero( &servaddr, sizeof(servaddr) );
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = 12345;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);

	int c_s = connect(sfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if(c_s < 0){
		close(sfd);
		perror("connect failed");
		exit(EXIT_FAILURE);
	}
	printf("successful connection\n");
	
	char* message = "lol";
	write(sfd, message, sizeof(message));

	close(sfd);
	return 0;
}



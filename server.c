#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h> //bzero
#include <unistd.h> // write - read

// SEE man 3 getaddrinfo client-server program example
int main(int argc, char* argv){

	setbuf(stdout, NULL);//pas besoin de fflush après printf

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket created\n");
	//fflush(stdout);	
	
	unsigned char buf[sizeof(struct in_addr)];
	int s = inet_pton(AF_INET, "127.0.0.1",buf);
	if (s <= 0){
		if(s == 0)
			fprintf(stderr, "Not in presentation format");
		else
			perror("inet pton");
		exit(EXIT_FAILURE);
	}
	
// C99
	/*const sockaddr_in servaddr = {
		.sin_family = AF_INET,
 		.sin_port = 12345,
		.sin_addr = "127.0.0.1",
		.sin_zero = 0
	 };*/
//C98
	/*const sockaddr_in servaddr = {AF_INET,12345,"127.0.0.1",0};*/
	
	struct sockaddr_in servaddr;
	bzero( &servaddr, sizeof(servaddr));
	servaddr.sin_family =  AF_INET;
	servaddr.sin_port = 12345;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
 
	/********************* BIND ****************************/

	int b_s = bind(sfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(b_s < 0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

 	printf("socket bound with return value %d\n", b_s);	 

	/********************* LISTEN ************************/

	int l_s = listen(sfd, 1/* nombre de connexions */);
	if(l_s < 0){
		perror("listen failed");
		exit(EXIT_FAILURE);
	}
	printf("listening with return value %d\n", l_s);	

	/******************** ACCEPT *****************************/
	int cfd = accept(sfd, (struct sockaddr *)NULL, NULL);  
	printf("connection acceptée avec socket %d\n", cfd);
	//fflush(stdout);

	char buff[40];
	read(cfd, buff, sizeof(buff));
	
	printf("message received: %s\n", buff);


	return 0;
}

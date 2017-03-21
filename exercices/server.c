#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h> //bzero
#include <unistd.h> // write - read

#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

/** 
* http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
*
*/
void handle_sigchld(int sig) {
  int saved_errno = errno;

  // -1 => wait for any child process
  while (waitpid((pid_t)(-1), 0, WNOHANG/* prevents handler from blocking */) > 0) {}
  /*
	man waitpid:
	"In the case of a terminated child, performing a wait  allows
	the  system  to  release  the resources associated with the child"
  */
  errno = saved_errno;
}

/**
*
* Ctrl+C
*
*/
void handle_sigint(int sig){
	//close sockets
}

/*
*
* @todo 
*
* @see man 3 getaddrinfo client-server program example
*
*
*
*/
int main(int argc, char* argv){

	/********************************************************************/

   /** 
	* http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
	* http://www-igm.univ-mlv.fr/~dr/CS/node178.html
	*/
	struct sigaction sa;
	sa.sa_handler = &handle_sigchld;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGCHLD, &sa, 0) == -1) {
		// echec de sigaction
		perror(0);
		exit(1);
	}

	/********************************************************************/

	setbuf(stdout, NULL);//pas besoin de fflush après printf

	int cfd;
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket created\n");
	//fflush(stdout);	

	/********************************************************************/

	/**
	* Fermer socket avant de quitter
	* via 'kill <pid>' par exemple
	*
	*/
	void handle_sigint_sigterm(int signo){
		if(signo == SIGINT){
			printf("SIGINT\n"); // CTRL+C
		}
		if(signo == SIGTERM){
			printf("SIGTERM\n");
		}
		close(sfd);
		exit(0);
	}

	struct sigaction act = {handle_sigint_sigterm};
	if (sigaction(SIGINT, &act, 0) == -1 || sigaction(SIGTERM, &act, 0) == -1) {
		// echec de sigaction
		perror(0);
		exit(1);
	}

	/********************************************************************/							

	
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

	while(1){

		cfd = accept(sfd, (struct sockaddr *)NULL, NULL);

		/******************* FORK *******************/

		int cpid = fork(); //return 0 in the son process
		if (cpid == -1) {
		   perror("fork");
		   exit(EXIT_FAILURE);
		}
		printf("connection acceptée avec socket %d\n", cfd);
		//fflush(stdout);

		/* Le fils ferme sfd qu'il n'utilisera plus */
		if(cpid == 0){
			close(sfd);

			char buff[40];
			read(cfd, buff, sizeof(buff)); // bloquant
			printf("server: message received: %s\n", buff);

			char* msg = "World";
			write(cfd, msg, sizeof(msg));

			// ATTENTION INDISPENSABLE !!
			break;

		/* Le père ferme cfd qu'il n'utilisera plus */
		}else{
			close(cfd);
		}

	}

	close(cfd);
	close(sfd);
	exit(0);
	return 0;
}

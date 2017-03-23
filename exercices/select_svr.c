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
#include <sys/select.h>
#include <errno.h>

#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))

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

#define BUF_SIZE 1024
#define MAX_CONNECTION 5
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

	char buf1[BUF_SIZE], buf2[BUF_SIZE];
	int cfd[MAX_CONNECTION] = {-1, -1, -1, -1, -1};
	int buf1_avail, buf1_written;
	int buf2_avail, buf2_written;
	int r;

	/**
	*
	* TODO check fd value before insert
	*
	*/
	int addCfd(int fd){
		if (fd == -1) {
        	perror("accept()");
        } else {
			int i;
			for(i = 0; i < MAX_CONNECTION; i++){
				if(cfd[i] == -1){
					cfd[i] = fd;
					i = -1;
					break;
				}
			}
			if(i != -1){
				close(fd);
				printf("Cannot accept connection: MAX_CONNECTION reached\n");
			}
		}
		return fd;
	}

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
		for(j = 0; j < MAX_CONNECTION; j++){
			fd = cfd[j];
			close(fd);
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

	fd_set rd, wr, er;
	int nfds, fd;
	FD_ZERO(&rd);
	FD_ZERO(&wr);
	FD_ZERO(&er);			
	FD_SET(sfd, &rd);
	FD_SET(sfd, &wr);
    nfds = max(nfds, sfd);
    int j;

	for(;;){

		r = select(nfds + 1, &rd, &wr, &er, NULL);
		if(r == -1){
			perror("select()");
       		exit(EXIT_FAILURE);
		}

		if(FD_ISSET(sfd, &rd)){
			
			printf("connection attempt\n");
			
		   /**
			* TODO enregistrer les infos du client
			*
			*/
			fd = addCfd( accept(sfd, (struct sockaddr *)NULL, NULL) );
			
			FD_SET(fd, &rd);
			FD_SET(fd, &wr);
			nfds = max(nfds, sfd);

		}
 
		for(j = 0; j < MAX_CONNECTION; j++){
			fd = cfd[j];
			if(fd != -1 && FD_ISSET(fd, &rd)){
				read(fd, buf1, sizeof(buf1));
				printf("server received: %s\n", buf1);
			}
			if(fd != -1 && FD_ISSET(fd, &wr)){
				char* msg = "World";
				write(fd, msg, sizeof(msg));
			}	
		}
	}

	for(j = 0; j < MAX_CONNECTION; j++){
		fd = cfd[j];
		close(fd);
	}
	close(sfd);
	exit(0);
	return 0;
}

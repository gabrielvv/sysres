#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int);

int main(int argc, char* argv){

	printf("PID %d\n", getpid());

	struct sigaction act = {handler};
	//act.sa_handler = handler;
	

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTSTP, &act, NULL);
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
	while(1){
	}
}

void handler(int signo){
	if(signo == SIGTSTP){
		printf("try SIGUSR\n");
	}
	if(signo == SIGINT){
		printf("nope! try SIGTSTP\n");
	}
	if(signo == SIGUSR1){
		printf("near\n");
	}
	if(signo == SIGUSR2){
		raise(SIGKILL);
	}
	fflush(stdout);	
}

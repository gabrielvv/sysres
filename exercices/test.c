#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handler(int);

int main(int argc, char* argv){

	printf("PID %d\n", getpid());

	struct sigaction act = {handler};
	printf("sigaction pointer %p\n", &act);	
	printf("sigaction pointer %p\n", act);	

	struct sigaction* act_p = malloc(sizeof(struct sigaction));
	printf("sigaction pointer %p\n", act_p);
	printf("sigaction pointer content %p\n", *act_p);
	//struct sigaction act = {handle};
	//printf("sigaction pointer %p\n", &act);
	
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

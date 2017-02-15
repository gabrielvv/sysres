#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv){

	printf("PID %d\n", getpid());

	sigset_t* set = malloc(sizeof(sigset_t));
	sigemptyset(set);
	sigaddset(set, SIGINT);
	printf("SIGINT is member: %d", sigismember(set, SIGINT));	
	sigprocmask(SIG_SETMASK, set, NULL);
	
	void handler(int signo){
		if(signo == SIGTSTP){
			
			sigset_t new_set;
			sigemptyset(&new_set);
			sigaddset(&new_set, SIGINT);
			sigprocmask(SIG_UNBLOCK, &new_set, NULL);
		}
		printf("SIG %d\n", signo);
		fflush(stdout);
	}
	signal(SIGTSTP, handler);
	//signal(SIGINT, handler);
	while(1){
	}
}

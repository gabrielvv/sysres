#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

int main(int argc, char* argv){

	printf("PID %d\n", getpid());
	fflush(stdout);

	void nope(){
		printf("nope, try Ctrl+Z\n");
		fflush(stdout);
	}

	void nope_bis(){
		printf("nope, try SIGUSR!\n");
		fflush(stdout);
	}

	void nope_ter(){
		printf("nope, you are near\n"); 
		fflush(stdout);
	}

	void ahhh(){
		raise(SIGKILL);	
	}


	signal(SIGINT,nope);
	signal(SIGTSTP,nope_bis);
	signal(SIGUSR1,nope_ter);
	signal(SIGUSR2,ahhh); 

	while(1){
//		raise(SIGINT);
//		raise(SIGTSTP);
//		raise(SIGUSR1);
//		raise(SIGUSR2);
	}	

}

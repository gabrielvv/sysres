#include <stdio.h>  //open
#include <fcntl.h>  // ??
#include <unistd.h> //write
#include <errno.h> //errno
#include <stdlib.h> //free
#include <string.h> //strlen

int main(int argc, char** argv){
	char* p = malloc(sizeof(char)*60);
	int n;

	errno = 0;
	n = scanf("%m[a-z]", &p);
	if(n == 1){ 
                   
		int fd = open("file1", O_RDWR | O_CREAT | O_APPEND, 0666);
		size_t len = strlen(p);
		write(fd, p, sizeof(char)*len);
		//read(fd, p, sizeof(char));
		printf("writing %s\n", p);
		free(p);
	} else if (errno != 0){
		perror("scanf");
	} else {
		fprintf(stderr, "No matching characters\n");
	}

	return 0;	
}

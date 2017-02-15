#include <stdio.h>  //open
#include <fcntl.h>  // ??
#include <unistd.h> //write
#include <errno.h> //errno
#include <stdlib.h> //free
#include <string.h> //strlen

int main(int argc, char** argv){
	char c;

	int from = open("to_revert", O_RDONLY, 0666);
	int to = open("reverted", O_WRONLY | O_CREAT, 0666); 
	
	off_t pos = lseek(from, -1, SEEK_END);

	while(pos>=0){
		
		read(from, &c, 1);
		write(to, &c, 1);
                pos = lseek(from, -2, SEEK_CUR);
	}

	return 0;	
}

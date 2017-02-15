#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	int fd1,fd2;
	char c;
	off_t pos;	

 	fd1=open("to_revert.txt",O_RDONLY);
	fd2=open("reverted.txt",O_WRONLY|O_CREAT,0666);

	pos=lseek(fd1,-1,SEEK_END);

	while(pos>=0)
	{
		read(fd1,&c,1);
		write(fd2,&c,1);
		pos=lseek(fd1,-2,SEEK_CUR);
	}

	return 0;
}

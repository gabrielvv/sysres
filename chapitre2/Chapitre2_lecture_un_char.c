#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for opening fd
#include <unistd.h> // for read and write and closing
int main(int argc, char** argv)
{
	int fd, nbecrits,nblus;
	char lettre;
	
	fd=open("data",O_RDONLY);
	nblus = read(fd,&lettre, sizeof(char));

	

	if (nblus<0)
	{
		printf("Erreur de lecture \n");
		exit(-1);
	}
	else if (nblus < sizeof(char))
	{
		printf("Pas assez de données à lure, fin du fichier atteinte\n");
	}
	
	printf(" Ma lettre : %c \n",lettre);
	return 0;
} 


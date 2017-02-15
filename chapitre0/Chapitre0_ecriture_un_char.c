#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for opening fd
#include <unistd.h> // for read and write and closing
int main(int argc, char** argv)
{
	int fd, nbecrits;
	char lettre;
	fd=open("data",O_RDWR | O_CREAT,0666);
	if (fd<0)
	{
		printf("Erreur d'ouverture\n");
		exit(-1);
	}
	printf("Entrez une valeur de caractère :");
	scanf("%c",&lettre);
	nbecrits=write(fd,&lettre,sizeof(char));
	// &lettre=adresse de lettre
	if (nbecrits<0)
	{
		printf("Erreur d'écriture\n");
		exit(-1);
	}
	else if (nbecrits<sizeof(char))
	printf("Disque plein\n");

	return 0;
}

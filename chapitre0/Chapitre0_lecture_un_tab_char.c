#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for opening fd
#include <unistd.h> // for read and write and closing
int main(int argc, char** argv)
{

	int fd,nbl,i;
	char tab[6];
	fd=open("my_first_tab",O_RDWR);
	if (fd<0)
	{
		printf("Erreur d'ouverture\n");
		exit(-1);
	}
	nbl=read(fd,tab,6*sizeof(char)); // le nom du tableau tab est aussi son adressedonc pas de &
	if (nbl<0)
	{	
		printf("Erreur de lecture\n");
		exit(-1);
	}
	else if (nbl<6*sizeof(int))
	{
			printf("Pas assez de données à lire : fin du fichier atteinte\n");
	}

	for (i=0;i<nbl;i++)
	printf("valeur lue n°%d : %c \n",i,tab[i]);

}
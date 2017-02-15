#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for opening fd
#include <unistd.h> // for read and write and closing

void clean_buffer()
{
	int c;
	while((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char** argv)
{
	int fd, nbecrits, i;
	char lettres[6];
	fd=open("my_first_tab",O_RDWR | O_CREAT,0666);
	if (fd<0)
	{
		printf("Erreur d'ouverture\n");
		exit(-1);
	}
	for (i = 0; i < 6 ; i++)	
	{
		printf("\nEntre la lettre numéro %d/6 : ",i+1);
		scanf("%c",&lettres[i]);	
		clean_buffer();
	}
	nbecrits=write(fd,lettres,6*sizeof(char));
	// ici lettres est l'adresse du tableau de lettres
	if (nbecrits<0)
	{
		printf("Erreur d'écriture\n");
		exit(-1);
	}
	else if (nbecrits<sizeof(char))
	printf("Disque plein\n");
	
	//on ferme proprement le fichier
	close(fd);


	return 0;
}

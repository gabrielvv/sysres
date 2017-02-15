#include <stdio.h>

int main(int argc, char** argv)
{
	FILE *fp;
	char chaine[3];
	fp=fopen("my_first_tab","r+");
	printf("Entrez une chaîne :");
	fgets(chaine,3,stdin);
	// lit une chaîne sur stdin de 3 caractères MAX ou jusqu'à rencontrer \n
	printf("%s",chaine);
	fclose(fp);
	fp=fopen("my_first_tab","r+");
	fputs(chaine,fp);
	// écrit la chaîne lue dans le fichier fp
	printf("%s",chaine);
	fclose(fp);
}

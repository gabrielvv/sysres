#include <stdio.h>
#include <stdlib.h>
struct etudiant
{
	char nom[20];
	int age;
	int num_id;
};

int main(int argc, char** argv)
{
	FILE* fp;
	struct etudiant etu[3];
	//Déclaration de trois structures
	int i;
	size_t nblus;
	fp=fopen("etudiantbinaire","w+");
	for (i=0;i<3;i++)
	{
		printf("Nom :");
		scanf("%s",&etu[i].nom);
		printf("Age :");
		scanf("%d",&etu[i].age);
		printf("ID de Classe :");
		scanf("%d",&etu[i].num_id);
	}
	nblus=fwrite(etu,sizeof(struct etudiant),3,fp);
	if (nblus<3)
	{
		printf("Erreur ou fin de fichier atteinte\n");
		exit(-1);
	}
	return 0;
}

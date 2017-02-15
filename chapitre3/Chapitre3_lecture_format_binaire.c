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
	struct etudiant etu;
	//Déetuaration de trois structures
	int i;
	size_t nblus;
	
	fp=fopen("etudiantbinaire","r");
	while ((nblus=fread(&etu,sizeof(struct etudiant),1,fp))==1)
	{
		printf("Nom : %s\n",etu.nom);
		printf("Age : %d\n",etu.age);
		printf("num_id : %d\n",etu.num_id);
	}
	return 0;
}
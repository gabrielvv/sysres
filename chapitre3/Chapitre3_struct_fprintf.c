#include <stdio.h>
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
	int i;
	size_t nblus;
	fp=fopen("essai_formate","w+");
	for (i=0;i<3;i++)
	{
		printf("Nom :");
		scanf("%s",&etu.nom);
		printf("Age :");
		scanf("%d",&etu.age);
		printf("num_id :");
		scanf("%d",&etu.num_id);
		fprintf(fp,"nom:%s, age:%d, num_id:%d\n",etu.nom,etu.age,etu.num_id);
	}
}
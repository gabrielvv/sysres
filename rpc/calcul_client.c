#include <limits.h>
#include "calcul.h"
CLIENT *clnt;

void test_addition(uint param1, uint param2)
{
	

	reponse  *resultat;
	data  parametre;

	/*1.Preparer les arguments*/
	parametre.arg1 = param1;
	parametre.arg2 = param2;
	printf("Appel de la fonction CALCUL_ADDITION avec les parametres:%u et %u\n",
		parametre.arg1,parametre.arg2);
	
	/*Appel de la fonction distante*/
	resultat = calcul_addition_1(&parametre, clnt);
	if(resultat==(reponse*)NULL){
		clnt_perror(clnt,"call failed");
		clnt_destroy(clnt);
		exit(EXIT_FAILURE);
	}
	else if(resultat->errno == 0){
		printf("Le resultat de l'addition est:%u \n\n",resultat->somme);
	}else{
		printf("la fonction distante ne peut faire l'addition à cause d'un overflow\n\n");
	}	
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	clnt = clnt_create(host, CALCUL, VERSION_UN,"udp");
	if(clnt==NULL){
		clnt_pcreateerror(host);
		exit(1);
	}

	test_addition(UINT_MAX - 15, 10);
	test_addition(UINT_MAX, 10);
	clnt_destroy(clnt);
	exit(EXIT_SUCCESS);
}

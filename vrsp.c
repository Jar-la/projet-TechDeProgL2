#include <libxml/tree.h>
#include <string.h>
#include "vrs.h"
#include "movie.h"
int menu(vrs_t vrs);

int main(int argc, char const *argv[])
{
	xmlDocPtr document;
	xmlNodePtr noeud;
	vrs_t *vrs1;
	movie_t * movie;
	int err;

	document = NULL;
	/*Charge le document*/
	document = xmlParseFile("./vrs.xml");
	if (document==NULL)
	{
		fprintf(stderr, "Erreur lors de l'ouverture du fichier\n");
		xmlCleanupParser(); /*Libère la mémoire allouée pour la bibliotheque elle-même */
		return(1);
	}

	/* noeud prend la racine du document */
	noeud = xmlDocGetRootElement(document);
	if(noeud == NULL)
	{
			fprintf(stderr, "Document vide");
			xmlFreeDoc(document);
			xmlCleanupParser();
			return 1;
	}

	/* Création d'un vidéo-club */
	vrs1 = vrs_create();
	if (vrs1 == NULL){
		fprintf(stderr, "memory allocation is a failure\n");
		return 1;
	}
	vrs1->movies = NULL;
	vrs1->nmovies = 0;

	/* Initialisation du nom du vidéo-club à partir du fichier xml */
	vrs1->name = (char *) xmlGetProp(noeud, (const xmlChar *) "name");

	noeud = noeud->xmlChildrenNode;
	noeud = noeud->next;

	/* Initialisation de la rue du vidéo-club à partir du fichier xml */
	vrs1->street =  (char *) xmlNodeListGetString(document, noeud->xmlChildrenNode, 1);

	/* Passage de <street> à <postal_code> (/!\Les retours à la ligne sont des noeuds) */
	noeud=noeud->next->next;

	/* Initialisation du code postal du vidéo-club à partir du fichier xml */
	vrs1->postal_code =  strtol((char *) xmlNodeListGetString(document, noeud->xmlChildrenNode, 1), NULL, 10);

	noeud=noeud->next->next;

	/* Initialisation de la ville du vidéo-club à partir du fichier xml */
	vrs1->city =  (char *) xmlNodeListGetString(document, noeud->xmlChildrenNode, 1);

	/*Rentrer dans <movies> */
	noeud = noeud->next->next->xmlChildrenNode->next;

	/* Parcourt <movies>, pour chaque <movie>, créé une structure movie_t et l'ajoute au vidéo-club */
	while(noeud != NULL)
	{
		if((xmlStrcmp(noeud->name, (const xmlChar*)"book")))
		{
			movie = movie_create();
			movie->name = (char *) xmlGetProp(noeud, (const xmlChar *) "name");
			movie->year = strtol((char *) xmlNodeListGetString(document, noeud->xmlChildrenNode->next->xmlChildrenNode,1),NULL,10);
			movie->price = strtod((char *) xmlNodeListGetString(document, noeud->xmlChildrenNode->next->next->next->xmlChildrenNode,1),NULL);
			err=vrs_add_movie(vrs1, movie);
			if (err==-1){
				fprintf(stderr, "memory allocation is a failure\n");
				return(1);
			}

		}
		noeud = noeud->next->next;
	}
	err= menu(*vrs1);
	if(err!=0){return 1;}
/**
	*Section TESTS

	vrs_handle_addr(*vrs1);
	vrs_handle_mv(*vrs1);
	vrs_handle_mvp(*vrs1, 1.99);
	vrs_handle_mvn(*vrs1, "Lord");

**/
	return 0;
}


int menu(vrs_t vrs){
	int i;
	int j;
	char s[30];
	char op[30];
	char val[30];
	do{
		i=0;
		j=0;
		/** Vider les cahines de char **/
		for(i=0;i<=30;i++){
			s[i] = '\0';
			op[i] = '\0';
			val[i] = '\0';
		}
		/** Affichage du prompt **/
		fprintf(stdout,"VRSP> ");

		/**On récupére s et on verifie de bien l'avoir reçu**/
		if( fgets(s,29, stdin) == NULL){ 
			fprintf(stderr,"Error: Failed to get user's imput");
			return 1;
		}

		/** Si NULL alors la commande n'a pas d'espace et donc pas de parmettres .**/
		if(strstr(s," ")==NULL){
			/** Pas de paramettres alors on copie s dans op **/
			i=0;
			while( s[i] != '\n' ){  
				op[i]=s[i];
				i++;
			}
		}else{
			/** On sépare la partie, commande et la valeur paramètre de la commande **/
			/**Tout les char jusqu'au premier espace sont l'operation**/
			i=0;
			j=0;
			while( s[i] != ' ' ){  
				op[i]=s[i];
				i++;
			}
			i++;
			/** Le reste est le parametre **/
			while( s[i] != '\n' ){  
				val[j]=s[i];
				i++;
				j++;
			}
			
		}
		if(i>18){
			fprintf(stdout,"Too many char for the command\n");
		}else{
			if(0==strcmp(op,"addr")){
				vrs_handle_addr(vrs);
			}else if(0==strcmp(op,"help\0")){
				fprintf(stdout,"addr: Prints the VRS address\n");
				fprintf(stdout,"help: Prints this help mv: Prints the VRS movies\n");
				fprintf(stdout,"mvn NAME: Prints the VRS movies containing the name NAME\n");
				fprintf(stdout,"mvp PRICE: Prints the VRS movies with the renting price equal to PRICE \n");
				fprintf(stdout,"mvpge PRICE: Prints the VRS movies with the renting price greater than or equal to PRICE\n");
				fprintf(stdout,"mvpgt PRICE: Prints the VRS movies with the renting price greater than PRICE\n");
				fprintf(stdout,"mvple PRICE: Prints the VRS movies with the renting price less than or equal to PRICE\n");
				fprintf(stdout,"mvplt PRICE: Prints the VRS movies with the renting price less than PRICE\n");	
				fprintf(stdout,"mvy YEAR: Prints the VRS movies with the release year equal to YEAR\n");
				fprintf(stdout,"mvyge YEAR: Prints the VRS movies with the release year greater than or equal to YEAR\n");
				fprintf(stdout,"mvygt YEAR: Prints the VRS movies with the release year greater than YEAR\n");
				fprintf(stdout,"mvyle YEAR: Prints the VRS movies with the release year less than or equal to YEAR\n");
				fprintf(stdout,"mvylt YEAR: Prints the VRS movies with the release year less than YEAR\n");
				fprintf(stdout,"version: Prints the VRSP version\n");
				fprintf(stdout, "quit: Quits VRSP\n");
			}else if(0==strcmp(op,"mv\0")){
				vrs_handle_mv(vrs);
			}else if(0==strcmp(op,"version\0")){
				fprintf(stdout,"VRSP (Video Rental Shop Program) 2018/12/01 \n");
			}else if(0==strcmp(op,"quit\0")){
				/**On fait rien**/
			}else if(j==0){
				fprintf(stdout,"Missing parameter for the %s command\n",op);
			}else if(0==strcmp(op,"mvn\0")){
				fprintf(stdout,"op=%s\n",op);
				fprintf(stdout,"val=%s\n",val);
				vrs_handle_mvn(vrs, val);
			}else if(0==strcmp(op,"mvp\0")){
					
			}else if (0==strcmp(op,"mvpg\0")){
				
			}else if (0==strcmp(op,"mvpgt\0")){
				
			}else if (0==strcmp(op,"mvple\0")){
				
			}else if (0==strcmp(op,"mvplt\0")){
				
			}else if (0==strcmp(op,"mvy\0")){
			
			}else if (0==strcmp(op,"mvyge\0")){
		
			}else if (0==strcmp(op,"mvygt\0")){
			
			}else if (0==strcmp(op,"mvyle\0")){
		
			}else if (0==strcmp(op,"mvylt\0")){

			}else{
				fprintf(stdout, "./vrsp.out: Invalid command \n");
			}
			}
		}while(!(0==strcmp(op,"quit\0")));
		return 0;
}
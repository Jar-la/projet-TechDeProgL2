#include <libxml/tree.h>
#include <string.h>
#include "vrs.h"
#include "movie.h"
int menu(vrs_t vrs);
int authorisees(char *op);

int main(int argc, char const *argv[])
{
	xmlDocPtr document;
	xmlNodePtr noeud,n ,n2;
	vrs_t *vrs1;
	movie_t * movie;
	int err;
	char* temp;
	if(argc!=2){
		fprintf(stderr,"./vrsp.out: Invalid number of arguments\n");
		return 1;
	}


	document = NULL;
	/*Charge le document*/
	document = xmlParseFile(argv[1]);
	if (document==NULL)
	{
		fprintf(stderr, "./vrsp.out: Unable to parse the document\n");
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

	noeud = noeud->children->next;

	/*Récupère l'addresse du vidéo-club */
	for (n=noeud; n != NULL; n=n->next){
		if(n->type != 3){
			if((xmlStrcmp(n->name, (const xmlChar*)"street"))==0)
			{
				vrs1->street =  (char *) xmlNodeListGetString(document, n->children, 1);
			}else if((xmlStrcmp(n->name, (const xmlChar*)"postal-code"))==0){
				temp = (char *) xmlNodeListGetString(document, n->children, 1);
				vrs1->postal_code =  strtol(temp, NULL, 10);
				free(temp);
			}else if ((xmlStrcmp(n->name, (const xmlChar*)"city"))==0){
				vrs1->city =  (char *) xmlNodeListGetString(document, n->children, 1);
			}
		}
	}

	/*Rentrer dans <movies> */
	while (xmlStrcmp(noeud->name, (const xmlChar*)"movies")!=0){
		noeud=noeud->next;
	}
	noeud=noeud->children;



	/* Parcourt <movies>, pour chaque <movie>, créé une structure movie_t et l'ajoute au vidéo-club */
	for (n=noeud; n != NULL; n=n->next){
		if(n->type != 3){
			if((xmlStrcmp(noeud->name, (const xmlChar*)"book")))
			{
				movie = movie_create();
				movie->name = (char *) xmlGetProp(n, (const xmlChar *) "name");
				n2=n->children;
				for (n2=n2; n2!= NULL; n2=n2->next)
				{
					if(n2->type != 3){
						if((xmlStrcmp(n2->name, (const xmlChar*)"year"))==0){
							temp = (char *) xmlNodeListGetString(document, n2->children,1);
							movie->year = strtol(temp,NULL,10);
							free(temp);
						}else if((xmlStrcmp(n2->name, (const xmlChar*)"price"))==0){
							temp = (char *) xmlNodeListGetString(document, n2->children,1);
							movie->price = strtod(temp,NULL);
							free(temp);
						}
					}
				}
				err=vrs_add_movie(vrs1, movie);
				if (err==-1){
					fprintf(stderr, "memory allocation is a failure\n");
					return(1);
				}
			}
		}
	}

	if(menu(*vrs1)!=0){return 1;}
	vrs_free(vrs1);
	xmlCleanupParser();
	xmlFreeDoc(document);
	return 0;
}


int menu(vrs_t vrs){
	int i;
	int j;
	double price;
    int year;
	char s[100];
	char op[100];
	char val[100];
	char *ptr;
	do{
		/** (Ré)Initialisation des variables **/
		i=0;
		j=0;
		price=0.0;
		year=0;
		/** (Ré)Initialisation des strings **/
		for(i=0;i<=100;i++){
			s[i] = '\0';
			op[i] = '\0';
			val[i] = '\0';
		}
		/** Affichage du prompt **/
		fprintf(stdout,"VRSP> ");
		/**On récupére s et on verifie de bien l'avoir reçu**/
		if( fgets(s,99, stdin) == NULL){
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
			/** Price et year respectivement float et int créés a partir de val**/
			price = strtod(val , &ptr);
  			year = strtod(val , &ptr);
		}

		/** Si  la commande dépasse 18 char erreur et retour a la saisie sinon on continue **/
		if(i>18){
			fprintf(stderr,"./vrsp.out: Too many characters for the command\n");
		}else{
			/** L'opérateur est-il dans la liste des oprérations authorisées **/
			if(authorisees(op)){
				fprintf(stderr, "./vrsp.out: Invalid command \n");
			}else if(0==strcmp(op,"addr")){
				vrs_handle_addr(vrs);
			}else if(0==strcmp(op,"help")){
				fprintf(stdout,"addr: Prints the VRS address\n");
				fprintf(stdout,"help: Prints this help\nmv: Prints the VRS movies\n");
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
			}else if(0==strcmp(op,"mv")){
				vrs_handle_mv(vrs);
			}else if(0==strcmp(op,"version")){
				fprintf(stdout,"VRSP (Video Rental Shop Program) 20181201 \n\n");
				fprintf(stdout,"Copyright (C) 2018 Edwin Vanootegem and Alexandre Lafon.\n\n");
				fprintf(stdout,"Written by Edwin Vanootegem <edwin.vanootegem@etud.univ-pau.fr> and Alexandre Lafon <alexandre.lafon@etud.univ-pau.fr>.\n");
			}else if(0==strcmp(op,"quit")){
				/** On ne fait rien **/
			/**  Les commande suivantes on besion d'un paramettre alors on regarde si j a fait un parcourt  **/
			}else if(j==0){
				fprintf(stderr,"./vrsp.out: Missing parameter for the %s command\n",op);
			}else if(0==strcmp(op,"mvn")){
				vrs_handle_mvn(vrs, val);
			/** ptr est une chaine contenant val privé de son nombre,
			si ptr est val sont identiques alors val ne contenait pas de nombre **/
			}else if(0==strcmp(val,ptr)){
				fprintf(stderr,"./vrsp.out: Invalid parameter for the %s command\n",op);
			}else if(0==strcmp(op,"mvp")){
				vrs_handle_mvp(vrs,price);
			}else if (0==strcmp(op,"mvpge")){
				vrs_handle_mvpge(vrs,price);
			}else if (0==strcmp(op,"mvpgt")){
				vrs_handlem_mvpgt(vrs,price);
			}else if (0==strcmp(op,"mvple")){
				vrs_handle_mvple(vrs,price);
			}else if (0==strcmp(op,"mvplt")){
				vrs_handle_mvplt(vrs,price);
			}else if (0==strcmp(op,"mvy")){
				vrs_handle_mvy(vrs,year);
			}else if (0==strcmp(op,"mvyge")){
				vrs_handle_mvyge(vrs,year);
			}else if (0==strcmp(op,"mvygt")){
				vrs_handle_mvygt(vrs,year);
			}else if (0==strcmp(op,"mvyle")){
				vrs_handle_mvyle(vrs,year);
			}else if (0==strcmp(op,"mvylt")){
				vrs_handle_mvylt(vrs,year);
			}
		}
	}while(!(0==strcmp(op,"quit\0")));
	return 0;
}
int authorisees(char *op){
	if( (0==strcmp(op,"addr")) || (0==strcmp(op,"help")) || (0==strcmp(op,"mv")) || (0==strcmp(op,"version")) || (0==strcmp(op,"quit")) || (0==strcmp(op,"mvn")) || (0==strcmp(op,"mvp")) || (0==strcmp(op,"mvpge")) || (0==strcmp(op,"mvpgt")) || (0==strcmp(op,"mvple")) || (0==strcmp(op,"mvplt")) || (0==strcmp(op,"mvy")) || (0==strcmp(op,"mvyge")) || (0==strcmp(op,"mvygt")) || (0==strcmp(op,"mvyle")) || (0==strcmp(op,"mvylt"))){
		return 0;
	}
	return -1;
}
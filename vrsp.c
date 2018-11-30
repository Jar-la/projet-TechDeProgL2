#include <libxml/tree.h>
#include "vrs.h"
#include "movie.h"

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
			if (err=-1){
				fprintf(stderr, "memory allocation is a failure\n");
				return(1);
			}

		}
		noeud = noeud->next->next;
	}

/**
	*Section TESTS
**/
	vrs_handle_addr(*vrs1);
	vrs_handle_mv(*vrs1);
	vrs_handle_mvp(*vrs1, 1.99);
	vrs_handle_mvn(*vrs1, "Lord");
	return 0;
}

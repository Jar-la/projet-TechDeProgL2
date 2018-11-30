#include <libxml/tree.h>
#include "vrs.h"
#include "movie.h"

int main(int argc, char const *argv[])
{
	xmlDocPtr document;
	xmlNodePtr noeud;
	vrs_t *vrs1;

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

	/* Initialisation du nom du vidéo-club à partir du fichier xml */
	vrs1->name = (char *) xmlGetProp(noeud, (const xmlChar *) "name");

	noeud = noeud->xmlChildrenNode;

	/* Initialisation de la rue du vidéo-club à partir du fichier xml */
	vrs1->street = (char *) xmlNodeListGetString(document, noeud, 1);

	noeud = noeud->next;
	/*
	vrs1->postal_code =  xmlNodeListGetString(document, noeud, 1);
	*En construction
 */
	return 0;
}

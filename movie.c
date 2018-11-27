#include "movie.h"
#include <stdio.h>
#include <stdlib.h>

movie_t *movie_create()
{
 	movie_t * nouv;
 	nouv = malloc(sizeof(movie_t));
 	return(nouv);
 }

void movie_free(movie_t *movie)
{
	free(movie);
}

void movie_handle_mv(movie_t movie)
{
	fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

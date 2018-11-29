#include "movie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

movie_t *movie_create()
{
 	movie_t * nouv = NULL;
 	nouv = malloc(sizeof(movie_t));
 	if (nouv == NULL)
 	{
 		fprintf(stderr, "L'allocation à échouée.\n");
 	}
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

void movie_handle_mvn(movie_t movie, const char * name)
{
	char * search;
	search = strstr(movie.name, name);
	if (search != NULL)
	{
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
	}
}

void movie_handle_mvp(movie_t movie, float price)
{
	if (price == movie.price)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvpge (movie_t movie, float price)
{
	if (price <= movie.price)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvpgt(movie_t movie, float price)
{
	if (price < movie.price)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvple(movie_t movie, float price)
{
	if (price >= movie.price)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvplt(movie_t movie, float price)
{
	if (price > movie.price)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvy(movie_t movie, int year)
{
	if (year == movie.year)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvyge(movie_t movie, int year)
{
	if (year <= movie.year)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvygt(movie_t movie, int year)
{
	if (year < movie.year)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvyle(movie_t movie, int year)
{
	if (year >= movie.year)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

void movie_handle_mvylt(movie_t movie, int year)
{
	if (year > movie.year)
		fprintf(stdout, "%s (%d), %f EUR\n", movie.name, movie.year, movie.price);
}

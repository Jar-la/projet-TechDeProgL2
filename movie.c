movie_t *movie_create()
{
 	movie_t nouv;
 	nouv = malloc(sizeof(movie_t));
 	return(nouv);
 }

void movie_free(movie_t *movie)
{
	free(movie);
}

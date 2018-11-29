#include "vrs.h" 
#include <stdio.h>
#include <stdlib.h>


int vrs_add_movie(vrs_t *vrs, movie_t *movie){
    vrs->movies = realloc(movie, (vrs->nmovies+1)*sizeof(movie_t));
    if (vrs->movies == NULL) return -1;
    vrs->nmovies++;
    return 0;
}

vrs_t *vrs_create(){
    return (malloc(sizeof(vrs_t)));
}

void vrs_free(vrs_t *vrs){
    free(vrs);
}

void vrs_handle_addr(vrs_t vrs){
    fprintf(stdout,"%s, %s ,%d , %s \n" ,vrs.name,vrs.street,vrs.postal_code,vrs.city);
}

void vrs_handle_mv(vrs_t vrs){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mv(*vrs.movies[i]);
    }
}

void vrs_handle_mvn(vrs_t vrs, const char *name){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvn(*vrs.movies[i],name);
    }
}

void vrs_handle_mvp(vrs_t vrs,float price){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvp(*vrs.movies[i],price);
    }
}

void vrs_handle_mvpge(vrs_t vrs, float price){
    int i;
    for (i=0; i<vrs.nmovies;i++){
         movie_handle_mvpge (*vrs.movies[i],price);
    } 
}

void vrs_handlem_mvpgt(vrs_t vrs, float price){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvpgt(*vrs.movies[i], price);
    } 
}

void vrs_handle_mvple(vrs_t vrs, float price){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvple(*vrs.movies[i],price);
    }     
}

void vrs_handle_mvplt(vrs_t vrs, float price){
     int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvplt( *vrs.movies[i], price);
    }        
}

void vrs_handle_mvy(vrs_t vrs, int year){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvy( *vrs.movies[i], year);
    }   
}

void vrs_handle_mvyge(vrs_t vrs, int year){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvyge( *vrs.movies[i] , year);
    }    
}
void vrs_handle_mvygt(vrs_t vrs, int year){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvygt( *vrs.movies[i] , year);
    }    
}

void vrs_handle_mvyle(vrs_t vrs, int year){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvyle( *vrs.movies[i] , year);
    }    
}

void vrs_handle_mvylt(vrs_t vrs, int year){
    int i;
    for (i=0; i<vrs.nmovies;i++){
        movie_handle_mvylt( *vrs.movies[i] , year);
    }      
}
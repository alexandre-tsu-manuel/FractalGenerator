#ifndef IFS_H_INCLUDED
#define IFS_H_INCLUDED

#include <SDL.h>

typedef struct element element;
struct element
{
    double matrix[2][3];
    struct element *nxt;
};
typedef element* llist;

typedef struct{
    float x, y;
}Point;

typedef struct{
    Point p[4];
}IFS_Quad;

void    IFS_set(SDL_Surface*, int);
void    IFS_add(double matrix[2][3], int); /* Ajoute une transformation */
int     IFS_setNumber(int n); /* Demande le nombre de polygones. */
void    IFS_draw(IFS_Quad, int); /* Dessine l'Ifs à l'écran */
void    IFS_close();

void GRAPH_line(int, int, int, int, int, int);
void GRAPH_plot(SDL_Surface*, int, int, Uint32);
void GRAPH_drawscreen();

#endif // IFS_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "IFS.h"

llist*          g_Matrix;  //Le tableau de matrices: 1 pour chaque quad
int             g_Quads; //Le nombre de quads utilisé
SDL_Surface*    g_Screen; //La surface sur laquelle on travaille.
int**           g_Buff;

Point matrix_multiply(Point p, int n)
{
    Point p2 = p, ptmp = p;
    int i, j;
    llist tmp = g_Matrix[n]; //On sauvegarde la première position.

    while(g_Matrix[n] != NULL)
    {
        /* On multiplie le point par les matrices */
        ptmp.x = p2.x * g_Matrix[n]->matrix[0][0] + p2.y * g_Matrix[n]->matrix[0][1] + g_Matrix[n]->matrix[0][2];
        ptmp.y = p2.x * g_Matrix[n]->matrix[1][0] + p2.y * g_Matrix[n]->matrix[1][1] + g_Matrix[n]->matrix[1][2];

        /* On pointe vers le prochain élément */
        g_Matrix[n] = g_Matrix[n]->nxt;

        /* On donne la nouvelle valeur a p2 */
        p2 = ptmp;
    }
    g_Matrix[n] = tmp; //On retourne au début

    return p2;
}

void recursive_proc(IFS_Quad quad, int n_iter)
{
    IFS_Quad *quad2 = malloc(g_Quads * sizeof(IFS_Quad));
    int i, j, k;

    /* Si on est a la dernière itération */
    if(n_iter == 0)
    {
        /* On déssine l'IFS */
        for(i = 0; i < 3; i++)
            GRAPH_line(quad.p[i].x, quad.p[i].y, quad.p[i+1].x, quad.p[i+1].y, g_Screen->w, g_Screen->h);
        GRAPH_line(quad.p[3].x, quad.p[3].y, quad.p[0].x, quad.p[0].y, g_Screen->w, g_Screen->h);

        /* Et on quitte */
        return;
    }

    /* Sinon, on multiplie les points par les matrices */
    for(i = 0; i < g_Quads; i++)
        for(j = 0; j < 4; j++)
            quad2[i].p[j] = matrix_multiply(quad.p[j], i);

    /* Et on lance le processus récursif */
    for(k = 0; k < g_Quads; k++)
        recursive_proc(quad2[k], n_iter-1);

    free(quad2);
}

void IFS_set(SDL_Surface *sur, int n)
{
    int i, j;

    g_Quads = n;
    g_Screen = sur;

    g_Matrix = (llist*)malloc(g_Quads * sizeof(llist));
    for(i = 0; i < g_Quads; i++)
        g_Matrix[i] = NULL;

    /* On alloue le double tableau correspondant à l'écran */
    g_Buff = malloc(g_Screen->w  * sizeof(int*));
    for(i = 0; i < g_Screen->w; i++)
        g_Buff[i] = malloc(g_Screen->h  * sizeof(int));

    /* Memset plantant, je l'ai fait barbare */
    for(i = 0; i < g_Screen->w; i++)
        for(j = 0; j < g_Screen->h; j++)
            g_Buff[i][j] = 0;
}

void IFS_add(double matrix[2][3], int n)
{
    /* On crée un nouvel élément */
    element* newmat = malloc(sizeof(element));
    memcpy(newmat->matrix, matrix, sizeof(double) * 3 * 2);
    newmat->nxt = NULL;
    int i, j;

    if(g_Matrix[n] == NULL)
        g_Matrix[n] = newmat;

    else
    {
        llist tmp = g_Matrix[n];

        while(tmp->nxt != NULL)
            tmp = tmp->nxt;

        tmp->nxt = newmat;
    }
}

void IFS_draw(IFS_Quad quad, int n_iter)
{
    recursive_proc(quad, n_iter);
    GRAPH_drawscreen();
}

void IFS_close()
{
    int i;
    for(i = 0; i < g_Screen->w; i++)
        free(g_Buff[i]);
    free(g_Buff);
    free(g_Matrix);
}


void GRAPH_plot(SDL_Surface *sur, int x, int y, Uint32 pixel)
{
    int bpp = sur->format->BytesPerPixel;

    if (x<0 || x>=sur->w|| y<0 || y>=sur->h)   // securité : si un pixel est dehors : on ne l'écrit pas.
        return;

    unsigned char *p = (unsigned char *)sur->pixels + y * sur->pitch + x * bpp;
    if (bpp==4)
        *(Uint32*)p = pixel;
}

void GRAPH_line(int x1, int y1, int x2, int y2, int sur_w, int sur_h)
{
    int x = x1,y = y1; /* piqué sur le site de fvirtman */
    int Dx,Dy;
    int xincr,yincr;
    int erreur;
    int i;

    Dx = abs(x2-x1);
    Dy = abs(y2-y1);
    if(x1<x2)   xincr = 1;
    else        xincr = -1;
    if(y1<y2)   yincr = 1;
    else        yincr = -1;

    if(Dx>Dy)
    {
        erreur = Dx/2;
        for(i=0;i<Dx;i++)
        {
            x += xincr;
            erreur += Dy;
            if(erreur>Dx)
            {
                erreur -= Dx;
                y += yincr;
            }
        if(x >= 0 && x < sur_w && y >= 0 && y < sur_h)
            g_Buff[x][y]++;
        }
    }
    else
    {
        erreur = Dy/2;
        for(i=0;i<Dy;i++)
        {
            y += yincr;
            erreur += Dx;
            if(erreur>Dy)
            {
                erreur -= Dy;
                x += xincr;
            }
        if(x >= 0 && x < sur_w && y >= 0 && y < sur_h)
            g_Buff[x][y]++;
        }
    }
}

void GRAPH_drawscreen()
{
    int i, j, max = 0;
    Uint8 r, g, b;

    for(i = 0; i < g_Screen->w; i++)
        for(j = 0; j < g_Screen->h; j++)
            max = (max > g_Buff[i][j]) ? max : g_Buff[i][j];

    for(i = 0; i < g_Screen->w; i++)
        for(j = 0; j < g_Screen->h; j++)
        {
            if(g_Buff[i][j] != 0)
                GRAPH_plot(g_Screen, i, j, SDL_MapRGB(g_Screen->format, 255*( pow((double)g_Buff[i][j]/max, -0.1/(double)g_Buff[i][j]/max ))
                                                                      , 255*( pow((double)g_Buff[i][j]/max, 0.4) )
                                                                      , 255*( pow((double)g_Buff[i][j]/max, (double)g_Buff[i][j]/max) )
                                                     ));
        }
}

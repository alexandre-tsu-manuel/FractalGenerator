#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>

#include "IFS.h"

void wait()
{
    SDL_Event event = {0};

    SDL_Flip(SDL_GetVideoSurface());
    while(event.type != SDL_QUIT && event.key.keysym.sym != SDLK_ESCAPE)
        SDL_WaitEvent(&event);
}

void init()
{
    SDL_Surface *screen;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        exit(0);

    screen = SDL_SetVideoMode(1366, 768, 32,SDL_SWSURFACE);
    if(screen == NULL)
        exit(0);
}

int main(int argc,char *argv[])
{
        /*** VARS ***/
    IFS_Quad  base  = {{{0, 0}, {1366, 0}, {1366, 768}, {0, 768}}};

        /*** SETUPS ***/
    init();
    SDL_Surface *screen = SDL_GetVideoSurface();

        /*** MAIN ***/
    double to_orig[2][3] = { {1, 0, -1000},
                             {0, 1, -1000} };
    double to_middle[2][3] = { {1, 0, 1000},
                               {0, 1, 1000} };

    double scale_1[2][3] = { {0.8, 0, 0},
                             {0, 0.8, 0} };
    double scale_2[2][3] = { {0.6, 0, 0},
                             {0, 0.6, 0} };

    double trans[2][3] = { {1, 0, 0},
                           {0, 1, 50} };


    double rot_1[2][3] =  { {cos(M_PI/4), -sin(M_PI/4), 0},
                              {sin(M_PI/4), cos(M_PI/4), 0} };

    IFS_set(screen, 2);

        IFS_add(to_orig, 0);
        IFS_add(scale_1, 0);
        IFS_add(rot_1, 0);
        IFS_add(to_middle, 0);
        IFS_add(trans, 0);

        IFS_add(scale_2, 1);

    IFS_draw(base, 20);

    wait();

        /*** QUIT ***/
    SDL_SaveBMP(screen, "out.bmp");
    SDL_Quit();
    IFS_close();
    return EXIT_SUCCESS;
}

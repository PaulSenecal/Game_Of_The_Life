/*
 Version:1.0
 Goal:game of the life
 Date:2017
 by:Paul_SNL
*/
#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <time.h>

int max(int i,int j){ return (i>j) ? i : j;}
int min(int i,int j){ return (i<j) ? i : j;}

int nb_vois(bool** cells,int sz, int i,int j){
    int nb_vois = 0;
    for(int i0 = max(i-1,0) ;i0 <= min (i+1,sz-1) ; i0++)
        for(int j0 = max(j-1,0) ; j0 <= min (j+1,sz-1) ; j0++)
            if(cells[i0][j0]) nb_vois++;
    return nb_vois - (cells[i][j]? 1 : 0);
}


int main ( int argc, char** argv ){
    SDL_Surface *blanc,*noir; int sz_rect = 5;
    blanc = SDL_CreateRGBSurface(SDL_HWSURFACE,sz_rect,sz_rect,32,0,0,0,0); SDL_FillRect(blanc, NULL , SDL_MapRGB(blanc->format, 255,255,255));
    noir = SDL_CreateRGBSurface(SDL_HWSURFACE,sz_rect,sz_rect,32,0,0,0,0);SDL_FillRect(noir, NULL , SDL_MapRGB(noir->format, 255,128,0));
    srand(time(NULL));
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }
    atexit(SDL_Quit);

    SDL_Surface* screen = SDL_SetVideoMode(800, 800, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen ){
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }


    bool done = false;
    int sz = 160;

    bool **cells,**change; cells = new bool*[sz];
    for(int i = 0 ; i < sz ; i++){
        cells[i] = new bool[sz];
        for(int j = 0 ; j < sz ; j++) cells[i][j] = rand() % 4 == 0;
    }


    while (!done){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type)
            {
            case SDL_QUIT:
                done = true;
                break;
            case SDL_KEYDOWN:{
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            }
        }
        change = new bool*[sz];
        for(int i = 0 ; i < sz ; i++){change[i] = new bool[sz];}

        for(int i = 0 ; i < sz ; i++){
            for(int j = 0 ; j < sz ; j++){
                int nb = nb_vois(cells,sz,i,j);
                change[i][j] = (cells[i][j] && (nb == 2 || nb == 3)) || (!cells[i][j] && nb == 3);
            }
        }
        for(int i = 0 ; i < sz ; i++){ delete[] cells[i];}
        delete[] cells;

        cells = change;

        SDL_Rect pos; pos.x = 0; pos.y = 0;

        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        for(int i = 0 ; i < sz ; i++){  pos.y = 0;
            for(int j = 0 ; j < sz ; j++){
                if(cells[i][j]) SDL_BlitSurface(noir, 0, screen, &pos);
                else SDL_BlitSurface(blanc, 0, screen, &pos);
                pos.y += sz_rect;
            }pos.x += sz_rect;
        }


        SDL_Flip(screen);
        SDL_Delay(100);
    }
    SDL_FreeSurface(blanc);
    SDL_FreeSurface(noir);
    printf("Exited cleanly\n");

    return 0;
}



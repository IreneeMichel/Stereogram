#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;
using std::vector;

#define SCREENHIGHT 700
#define SCREENWIDTH 900
#include "Monde.h"

//void myflush ( FILE *in )
//{
//  int ch;
//
//  do
//    ch = fgetc ( in );
//  while ( ch != EOF && ch != '\n' );
//
//  clearerr ( in );
//}
//
//void mypause ( void )
//{
//  printf ( "Press [Enter] to continue . . ." );
//  fflush ( stdout );
//  getchar();
//}

int main (int argc, char** argv)
{
   SDL_Event e;
     SDL_Window* window = NULL;
    window = SDL_CreateWindow
    (
        "labyrinthe 3D", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREENWIDTH,
        SCREENHIGHT,
        SDL_WINDOW_SHOWN
    );
    // Setup renderer
    SDL_Renderer* renderer = NULL;
    renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

    // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
    SDL_Rect r,r2;
    r.x = 50;
    r.y = 150;
    r.w = 50;
    r.h = 50;
    int distance =250 ;
    int level=4 ;
    cout << " set red rectangles with arrows to correspond to the distance between your eyes\n" ;
    cout << "try to look beyond until you see 3 rectangles \n" ;
    cout << "then touch <SPACE> to enter the maze" << endl ;
    // Set render color to blue ( rect will be rendered in this color )
    bool quit = false;
    while( !quit ) {
      while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ) { quit = true; }
        else if( e.type == SDL_KEYDOWN ) {
        //Select surfaces based on key press
          switch( e.key.keysym.sym ) {
                                       case SDLK_LEFT: cout << "left" << endl ; distance-=10 ;break;
                                       case SDLK_RIGHT: cout << "right" << endl ; distance+=10 ; break;
                                       case SDLK_SPACE: cout << "space" << endl ; quit=true ; break;
                                       case SDLK_UP: level++ ; cout << "level "<<level << endl  ; break;
                                       case SDLK_DOWN: if (level>0) level-- ; cout << "level "<<level << endl  ; break;
                                    default: cout  << " other \n"; break;
                                       }
                                   }
                               }
     SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
    SDL_RenderClear( renderer );
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
   SDL_RenderFillRect( renderer, &r );
    r2.x = 50+distance;
    r2.y = 150;
    r2.w = 50;
    r2.h = 50;
    SDL_RenderFillRect( renderer, &r2 );
     SDL_RenderPresent(renderer);
  }
    // Render rect

    // Render the rect to the screen

    SDL_Texture *sdlTexture ;
    sdlTexture = SDL_CreateTexture(renderer,
                               SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               SCREENWIDTH, SCREENHIGHT);
    Uint32 *myPixels = new Uint32[SCREENWIDTH*SCREENHIGHT];

    // Wait for 5 sec
    //Main loop
     quit = false;
    Monde monde=Monde(level) ;
    monde.setScreenSize(SCREENWIDTH,SCREENHIGHT,distance) ;
    monde.getImage(myPixels);

    cout << "\n use a,q ,and arrows to move in the maze\n" << endl ;
    //While application is running
    while( !quit ) {
      while( SDL_PollEvent( &e ) != 0 ) {
        if( e.type == SDL_QUIT ) { quit = true; }
        else if( e.type == SDL_KEYDOWN ) {
        //Select surfaces based on key press
          switch( e.key.keysym.sym ) { case SDLK_UP:
                                            cout << "up" << endl ;monde.turnUp() ; break;
                                       case SDLK_DOWN: cout << "down" << endl ; monde.turnDown() ; break;
                                       case SDLK_LEFT: cout << "left" << endl ; monde.turnLeft() ; break;
                                       case SDLK_RIGHT: cout << "right" << endl ;monde.turnRight() ; break;
                                       case SDLK_a: cout << "forward" << endl ;monde.moveForward() ; break;
                                       case SDLK_q: cout << "back" << endl ;monde.moveBack() ; break;
                                      case SDLK_m: monde.switchMap() ; break;
                                       default: cout  << " defaut \n"; monde.move() ; break;
                                       }
                            monde.getImage(myPixels);
                            }
                        }
    //cout << " main \n" ;
    SDL_UpdateTexture(sdlTexture, NULL, myPixels, SCREENWIDTH * sizeof (Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
//         while( SDL_PollEvent( &e ) == 0 ) {
//                               }
   }
     SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


/*
	Duck Hunt is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	any later version.

	Duck Hunt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	Design copyright (C) 1985-2015 Nintendo
	
	Developed in C++ by:
		- Bisi Simone 	 [ bisi.simone (at) gmail (dot) com ]
		- Carpino Andrea [ carpocci (at) gmail (dot) com ]
	for studying purposes ONLY, between 2014/2015.
*/

#include <iostream>
using namespace std;

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#include "image_man.h"

#include "var_init.h"

#include "dog.h"
#include "duck.h"

//******************************************************************************

/** Immagine del cursore
 * Da: http://buildbot.wz2100.net/files/doxygen/cursors__sdl_8cpp_source.html
 */
static const char *pointer[] =
{
	// width height num_colors chars_per_pixel
	"    32    32        3            1",
	// colors
	"X c #000000",
	". c #ffffff",
	"  c None",
	//pixels
	"                                ",
	"                                ",
	"                                ",
	"               .                ",
	"               .X               ",
	"            ....X..             ",
	"          ......X....           ",
	"         ...XXX.XXX...          ",
	"        ..XX   .X   X..         ",
	"       ..X     .X     ..        ",
	"      ..X      XX      ..       ",
	"      ..X              ..X      ",
	"     ..X                ..X     ",
	"     ..X                ..X     ",
	"     ..X                ..X     ",
	"   ........X   .    ........X   ",
	"    XXXXXXXX   X     XXXXXXXX   ",
	"     ..X                ..X     ",
	"     ..X                ..X     ",
	"     ..X                ..X     ",
	"      ..                .X      ",
	"      ..X      .       ..X      ",
	"       ..      .X     ..X       ",
	"        ..     .X    ..X        ",
	"         ...   .X   ..X         ",
	"         X......X....X          ",
	"          XX....X..XX           ",
	"            XXX.XXX             ",
	"               .X               ",
	"               XX               ",
	"                                ",
	"                                ",
	"15,15"
};

//******************************************************************************

/** Funzione per generare il cursore da una matrice di caratteri
 * Da: https://wiki.libsdl.org/SDL_CreateCursor
 /param image array di stringhe con cui creare il cursore.
*/
static SDL_Cursor *init_system_cursor(const char *image[])
{
	int i, row, col;
	Uint8 data[4*32];
	Uint8 mask[4*32];
	int hot_x, hot_y;
	
	i = -1;
	for ( row=0; row<32; ++row )
	{
		for ( col=0; col<32; ++col )
		{
			if ( col % 8 )
			{
				data[i] <<= 1;
				mask[i] <<= 1;
			}
			else
			{
				++i;
				data[i] = mask[i] = 0;
			}
			
			switch (image[4+row][col])
			{
				case 'X':
					data[i] |= 0x01;
					mask[i] |= 0x01;
					break;
					
				case '.':
					mask[i] |= 0x01;
					break;
					
				case ' ':
					break;
			}
		}
	}
	sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
	return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}

//******************************************************************************

/** Gestisce il caricamento delle risorse e l'avvio di SDL. */
bool init()
{
    //Inizializza tutte i sistemi SDL
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        cerr << "Errore nel richiamo della funzione SDL_Init()." << endl;
        return false;
    }
    
    //Setup screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    if( screen == NULL )
    {
        cerr << "Errore nel richiamo della funzione SDL_SetVideoMode()." << endl;
        return false;
    }
    
    //Inizializza SDL_Mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    {
        cerr << "Errore nel richiamo della funzione Mix_OpenAudio()." << endl;
        return false;
    }
    
     //Inizializza SDL_TTF
    if( TTF_Init() == -1 )
    {
		cerr << "Errore nel richiamo della funzione TTF_Init()." << endl;
        return false;
    }
    
    //Titolo dell'applicazione
    SDL_WM_SetCaption( "Duck Hunt", NULL );

	//Cursore
    cursor = init_system_cursor(pointer);
    SDL_SetCursor(cursor);
    SDL_ShowCursor(0);
    
    dog_set_clips();
	
    return true;
}

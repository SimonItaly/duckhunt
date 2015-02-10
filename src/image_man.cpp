
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
#include <string>
using namespace std;

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "image_man.h"

//******************************************************************************

SDL_Surface *load_image( string filename )
{
    //Immagine iniziale
    SDL_Surface* loadedImage = NULL;

    //Immagine finale
    SDL_Surface* optimizedImage = NULL;

    //Caricamento dell'immagine
    loadedImage = IMG_Load( filename.c_str() );
    if( loadedImage != NULL )
    {
        //Crea l'immagine finale
        optimizedImage = SDL_DisplayFormat( loadedImage );
        if( optimizedImage != NULL )
        {
            //Applica la color key
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }

        //Rimuovi l'immagine iniziale.
        SDL_FreeSurface( loadedImage );
    }

    //Ritorna l'immagine finale
    return optimizedImage;
}

int SDL_FillRect(SDL_Surface* dst, const int x, const int y, const int h, const int w, Uint32 color)
{
	//Converte gli offset da interi a SDL_Rect
	SDL_Rect offset;
	offset.x = x;
    offset.y = y;
    offset.h = h;
    offset.w = w;

    return SDL_FillRect(dst, &offset, color);
}

int apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Converte gli offset da interi a SDL_Rect
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    return SDL_BlitSurface( source, clip, destination, &offset );
}

int apply_surface( SDL_Rect r, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Blit
    return SDL_BlitSurface( source, clip, destination, &r );
}


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
#include "timer.h"

#include "var_init.h"
#include "resources.h"

#include "dog.h"

//******************************************************************************

const int
	dog_w = 112, /**< Larghezza dello spritesheet del cane. */
	dog_h = 105; /**< Altezza dello spritesheet del cane. */

/** Vettore delle aree di ritaglio degli sprites del cane. */
SDL_Rect frames[11];

/** Struttura contenente le variabili utilizzate per animare il cane. */
struct dog
{
	int x_offset; /**< posizione orizzontale attuale del cane. */
	int y_offset; /**< posizione verticale attuale del cane. */
	int velocity; /**< la velocità con la quale si muove il cane. */
	int frame; /**< il frame attuale. */
	int sniff_n;/**< contatore dei frame dell'animazione di annusata. */
	int walk_n; /**< contatore dei frame dell'animazione di camminata. */
    int ani_count; /**< contatore dei frame inviati a schermo. */
} d;

//******************************************************************************

/** Avanza al frame successivo. */
void step_frame()
{
	if( d.frame == 2 )
	{
		d.walk_n++;
		d.frame = 0;
	}
	else
	{
		if( d.frame == 4 )
		{
			d.sniff_n++;
			d.frame = 3;
		}
		else
		{
			d.frame++;
		}
	}

	if( d.walk_n > 4 )
	{
		d.walk_n = 0;
		d.velocity = 0;
		d.frame = 3;
	}

	if( d.sniff_n > 2 )
	{
		d.sniff_n = 0;
		d.velocity = 3;
		d.frame = 0;
	}
}

//******************************************************************************

void dog_set_clips()
{
    //sniff 1
    frames[ 4 ].x = 0;
    frames[ 4 ].y = 0;
    frames[ 4 ].w = dog_w;
    frames[ 4 ].h = 86;

    //sniff 2
    frames[ 3 ].x = dog_w;
    frames[ 3 ].y = 0;
    frames[ 3 ].w = dog_w;
    frames[ 3 ].h = 86;

    //walk 1
    frames[ 0 ].x = dog_w * 2;
    frames[ 0 ].y = 0;
    frames[ 0 ].w = dog_w;
    frames[ 0 ].h = 86;

    //walk 2
    frames[ 1 ].x = dog_w * 3;
    frames[ 1 ].y = 0;
    frames[ 1 ].w = dog_w;
    frames[ 1 ].h = 86;

    //walk 3
    frames[ 2 ].x = dog_w * 4;
    frames[ 2 ].y = 0;
    frames[ 2 ].w = dog_w;
    frames[ 2 ].h = 86;

    //surprise
    frames[ 5 ].x = 0;
    frames[ 5 ].y = dog_h;
    frames[ 5 ].w = 108;
    frames[ 5 ].h = dog_h;

	//jump 1
    frames[ 6 ].x = 122;
    frames[ 6 ].y = dog_h;
    frames[ 6 ].w = 70;
    frames[ 6 ].h = dog_h;

	//jump 2
    frames[ 7 ].x = 206;
    frames[ 7 ].y = dog_h;
    frames[ 7 ].w = 66;
    frames[ 7 ].h = dog_h;

    //laugh 1
    frames[ 8 ].x = 502;
    frames[ 8 ].y = dog_h;
    frames[ 8 ].w = 58;
    frames[ 8 ].h = dog_h;

    //laugh 2
    frames[ 9 ].x = 564;
    frames[ 9 ].y = dog_h;
    frames[ 9 ].w = 58;
    frames[ 9 ].h = dog_h;

    //1 duck
    frames[ 10 ].x = 300;
    frames[ 10 ].y = dog_h;
    frames[ 10 ].w = 86;
    frames[ 10 ].h = dog_h;
}

//******************************************************************************

void dog_init()
{
	//Initialize movement variables
    d.x_offset = 0;
	d.y_offset = 104;
    d.velocity = 3;

    d.frame = 0;

	d.walk_n = 0;
	d.sniff_n = 0;
	d.ani_count = 0;
}

void dog_move()
{
	//Movimento
    d.x_offset += d.velocity;

    //Mantieni nei limiti
    if( ( d.x_offset < 0 ) || ( d.x_offset + dog_w > SCREEN_WIDTH ) )
    {
        d.x_offset -= d.velocity;
    }
}

bool dog_show()
{
	apply_surface( d.x_offset, ( SCREEN_HEIGHT - d.y_offset - frames[ d.frame ].h ), dog_sprites, screen, &frames[ d.frame ] );

	if( d.ani_count > 76 )
		return true;
	return false;
}

bool dog_intro_animation()
{
	++d.ani_count;
	switch( d.ani_count )
	{
		case 0 ... 61:
			step_frame();
			dog_move();
			SDL_Delay( 120 );
			break;

		case 62 ... 64:
			d.frame = 5;
			SDL_Delay( 120 );
			break;

		case 65:
			Mix_PlayChannel( -1, bark, 0 );

		case 66 ... 70:
			d.frame = 6;
			d.x_offset += 5;
			d.y_offset += 9;
			break;

		case 71 ... 75:
			d.frame = 6;
			d.x_offset += 5;
			d.y_offset += 4;
			break;

		case 76 ... 90:
			d.frame = 7;
			d.x_offset += 5;
			d.y_offset -= 6;
			break;

		default:
			return false;
			break;
	}

	return true;
}

//******************************************************************************

void dog_catch_init()
{
	d.frame = 0;
	d.y_offset = 230;
}

bool dog_catch_anim( int x )
{
	apply_surface( x, d.y_offset, dog_sprites, screen, &frames[ 10 ] );
	apply_surface( 0, 0, grass_over, screen );

	if(d.frame < 180 && d.y_offset > 190)
	{
		d.y_offset--;
		SDL_Delay( 2 );
	}
	else
	{
		if(d.frame >= 240)
			return false;
		else if(d.frame >= 180)
			d.y_offset++;

		d.frame++;
	}
	return true;
}

//******************************************************************************

void dog_lol_init()
{
	d.y_offset = 230;
	d.frame = 8;
}

void dog_show_lol( bool back )
{
	apply_surface( SCREEN_WIDTH / 2 - ( frames[ d.frame ].w / 2 ), d.y_offset, dog_sprites, screen, &frames[ d.frame ] );

	if(back && d.y_offset < 257)
	{
		d.y_offset++;
		SDL_Delay( 2 );
	}
	else if(d.y_offset > 190)
	{
		d.y_offset--;
		SDL_Delay( 2 );
	}
	else
	{
		if( ++ d.frame >= 10 )
			d.frame = 8;
		SDL_Delay( 150 );
	}
}

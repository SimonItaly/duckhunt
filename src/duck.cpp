
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
#include <ctime>
#include <cmath>
using namespace std;

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include "image_man.h"

#include "var_init.h"
#include "resources.h"

#include "duck.h"

//******************************************************************************

/** Vettore delle aree di ritaglio degli sprites delle papere. */
SDL_Rect duck_frames[12];

/** Array di immagini. */
SDL_Surface *sprite_sheet[3];

const int
    clip_w = 228 / 3, /**< Larghezza dello spritesheet della papera. */
    clip_h = 296 / 4 /**< Altezza dello spritesheet della papera. */
;

/** Tipi di papere. */
const int
	BLACK_DUCK = 0,
	RED_DUCK = 1,
	BLUE_DUCK = 2
;

/** Velocità di base delle papere. */
const int
	DUCK_SPEED = 10;

/** True se i bordi degli sprite delle papere sono già stati settati. */
bool duck_clips_set = false;

//******************************************************************************

void duck_set_clips()
{
	if(!duck_clips_set)
	{
		sprite_sheet[0] = black_duck;
		sprite_sheet[1] = red_duck;
		sprite_sheet[2] = blue_duck;

		//volo orizzontale
		duck_frames[0].x = 0 ;
		duck_frames[0].y = 0 ;
		duck_frames[0].w = clip_w ;
		duck_frames[0].h = clip_h ;

		duck_frames[1].x = clip_w ;
		duck_frames[1].y = 0 ;
		duck_frames[1].w = clip_w ;
		duck_frames[1].h = clip_h ;

		duck_frames[2].x = 2 * clip_w ;
		duck_frames[2].y = 0 ;
		duck_frames[2].w = clip_w ;
		duck_frames[2].h = clip_h ;

		//volo diagonale
		duck_frames[3].x = 0 ;
		duck_frames[3].y = clip_h ;
		duck_frames[3].w = clip_w ;
		duck_frames[3].h = clip_h ;

		duck_frames[4].x = clip_w ;
		duck_frames[4].y = clip_h ;
		duck_frames[4].w = clip_w ;
		duck_frames[4].h = clip_h ;

		duck_frames[5].x = 2 * clip_w ;
		duck_frames[5].y = clip_h ;
		duck_frames[5].w = clip_w ;
		duck_frames[5].h = clip_h ;

		//volo verticale
		duck_frames[6].x = 0 ;
		duck_frames[6].y = 2* clip_h ;
		duck_frames[6].w = clip_w ;
		duck_frames[6].h = clip_h ;

		duck_frames[7].x = clip_w ;
		duck_frames[7].y = 2 * clip_h ;
		duck_frames[7].w = clip_w ;
		duck_frames[7].h = clip_h ;

		duck_frames[8].x = 2 * clip_w ;
		duck_frames[8].y = 2 * clip_h ;
		duck_frames[8].w = clip_w ;
		duck_frames[8].h = clip_h ;

		//shot!
		duck_frames[9].x = 0 ;
		duck_frames[9].y = 3* clip_h ;
		duck_frames[9].w = clip_w ;
		duck_frames[9].h = clip_h ;

		//caduta
		duck_frames[10].x = clip_w ;
		duck_frames[10].y = 3 * clip_h ;
		duck_frames[10].w = clip_w ;
		duck_frames[10].h = clip_h ;

		duck_frames[11].x = 2 * clip_w ;
		duck_frames[11].y = 3 * clip_h ;
		duck_frames[11].w = clip_w ;
		duck_frames[11].h = clip_h ;

		duck_clips_set = true;
	}
}

//******************************************************************************

void duck_set_dest( duck &d )
{
	//Sinistra
	if ( d.x_offset < 5 )
	{
		d.angle = 270 + (rand() % 180);
	}
	//Destra
	else if ( d.x_offset + duck_frames[ d.frame ].w > SCREEN_WIDTH )
	{
		d.angle = 90 + (rand() % 180);
	}
	//Alto
	else if ( d.y_offset < 5 )
	{
		d.angle = 0 + (rand() % 180);
	}
	//Basso
	else if ( d.y_offset + duck_frames[ d.frame ].h > 270 )
	{
		d.angle = 180 + (rand() % 180);
	}

	//No angoli > 360
	d.angle = d.angle % 360;
}

void duck_init( duck &d, int speed )
{
	//Random seed
	srand( SDL_GetTicks() );

	//Frame dell'animazione
    d.frame = 0;

    //Contatore animazioni
	d.counter = 0;

    //Posizione iniziale
	d.x_offset = rand() % SCREEN_WIDTH;
    d.y_offset = 200;

    //Direzione iniziale verso destra
    d.angle = ( 270 + (rand() % 90) ) % 360;

    //Velocità, dipendente dal round corrente
    d.speed = DUCK_SPEED + speed;

	//Fly away
	d.away = false;

    //Inizializza il tipo di papera
	switch( rand() % 3 )
	{
		case 0:
			d.type = BLACK_DUCK;
			d.points = 500;
			break;
		case 1:
			d.type = RED_DUCK;
			d.points = 1500;
			break;
		case 2:
			d.type = BLUE_DUCK;
			d.points = 1000;
			break;
		default:
			break;
	}

	duck_set_clips();
}

bool duck_shot_anim( duck &d )
{
	//Mostra il frame della papera colpita
	if(d.frame < 9 || d.counter < 20)
	{
		d.frame = 9;
		d.counter++;
	}
	else
	{
		//Alterna l'animazione di caduta
		if(d.y_offset % 10 == 0)
		{
			if(d.frame == 11)
				d.frame = 10;
			else
				d.frame = 11;
		}
		d.y_offset ++;
	}

	apply_surface( d.x_offset, d.y_offset, sprite_sheet[ d.type ], screen, &duck_frames[ d.frame ] );
	apply_surface( 0, 0, grass_over, screen );

	//Ritorna true se si trova sotto l'erba
	if(d.y_offset < 260)
		return true;

	return false;
}

void duck_move( duck &d, bool fly_away )
{
	//Movimento in base all'angolo
	//La costante double 0.0174532925 permette la conversione
	//da radianti a gradi di cos() e sin()
	d.x_offset = d.x_offset + int(d.speed * cos((double)d.angle * 0.0174532925));
	d.y_offset = d.y_offset + int(d.speed * sin((double)d.angle * 0.0174532925));

	//Se è volata via procedi fino a -300 per farla sparire dallo schermo
	if(fly_away)
	{
		if(d.y_offset < -300)
			d.away = true;
	}
	else if( d.x_offset < 5  ||
			 d.x_offset + duck_frames[ d.frame ].w > SCREEN_WIDTH ||
			 d.y_offset < 5 ||
			 d.y_offset + duck_frames[ d.frame ].h > 270 )
	{
		//Se ha raggiunto la destinazione ricalcola l'angolo
		duck_set_dest( d );
	}
}

void duck_show( duck &d )
{
	//Animazione movimento
	d.counter++;
	if(d.counter)
	{
		d.counter = 0;

		d.frame++;
		switch(d.angle)
		{
			case 45 ... 224:	//Diagonale	3,4,5
			{
				if(d.frame < 3 || d.frame > 5)
					d.frame = 3;
				break;
			}
			case 225 ... 315:	//Verticale 6,7,8
			{
				if(d.frame < 6 || d.frame > 8)
					d.frame = 6;
				break;
			}
			default: 			//Orizzontale 0,1,2
			{
				if(d.frame > 2)
					d.frame = 0;
				break;
			}
		}
	}

    apply_surface( d.x_offset, d.y_offset, sprite_sheet[ d.type ], screen, &duck_frames[ d.frame ] );
    apply_surface( 0, 0, grass_over, screen );

	SDL_Delay( 50 );
}

bool duck_range_check( const duck d, int x, int y )
{
	return
	(
		(x >= d.x_offset && x <= d.x_offset + clip_w)
		&&
		(y >= d.y_offset && y <= d.y_offset + clip_h)
	);
}


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
#include <iomanip>
#include <sstream>
using namespace std;

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#include "image_man.h"

//******************************************************************************

SDL_Surface *menu = NULL;
SDL_Surface *arrow = NULL;
SDL_Surface *duck_bg = NULL;
SDL_Surface *dog_sprites = NULL;
SDL_Surface *black_duck = NULL;
SDL_Surface *red_duck = NULL;
SDL_Surface *blue_duck = NULL;
SDL_Surface *pass_lines = NULL;
SDL_Surface *bullet = NULL;
SDL_Surface *perfect = NULL;
SDL_Surface *round_box = NULL;
SDL_Surface *game_over_box = NULL;
SDL_Surface *grass_over = NULL;
SDL_Surface *fly_away_box = NULL;

//******************************************************************************

Mix_Music *clay_intro = NULL;
Mix_Music *duck_intro = NULL;
Mix_Music *flap = NULL;
Mix_Music *menu_sound = NULL;

//******************************************************************************

Mix_Chunk *bark = NULL;
Mix_Chunk *clear = NULL;
Mix_Chunk *duck_get = NULL;
Mix_Chunk *game_over_sound = NULL;
Mix_Chunk *miss = NULL;
Mix_Chunk *perfect_sound = NULL;
Mix_Chunk *shoot = NULL;
Mix_Chunk *move_duck_sound = NULL;
Mix_Chunk *falling = NULL;
Mix_Chunk *bump = NULL;

//******************************************************************************

TTF_Font *font = NULL;

//******************************************************************************

#include "xpm/title.xpm"
#include "xpm/menu_arrow.xpm"
#include "xpm/background.xpm"
#include "xpm/dog_sprites.xpm"
#include "xpm/black_duck.xpm"
#include "xpm/red_duck.xpm"
#include "xpm/blue_duck.xpm"
#include "xpm/pass_lines.xpm"
#include "xpm/bullet.xpm"
#include "xpm/perfect.xpm"
#include "xpm/round_box.xpm"
#include "xpm/game_over_box.xpm"
#include "xpm/grass_over.xpm"
#include "xpm/fly_away.xpm"

/** Gestisce il caricamento delle immagini del gioco.
 * \return false in caso di errore, true altrimenti.
 */
bool load_images()
{
	menu = IMG_ReadXPMFromArray( ( char ** ) ( title_xpm) );
	arrow = IMG_ReadXPMFromArray( ( char ** ) ( menu_arrow_xpm) );
	duck_bg = IMG_ReadXPMFromArray( ( char ** ) ( background_xpm) );
	dog_sprites = IMG_ReadXPMFromArray( ( char ** ) ( dog_sprites_xpm) );
	black_duck = IMG_ReadXPMFromArray( ( char ** ) ( black_duck_xpm) );
	red_duck = IMG_ReadXPMFromArray( ( char ** ) ( red_duck_xpm) );
	blue_duck = IMG_ReadXPMFromArray( ( char ** ) ( blue_duck_xpm) );
	pass_lines = IMG_ReadXPMFromArray( ( char ** ) ( pass_lines_xpm) );
	bullet = IMG_ReadXPMFromArray( ( char ** ) ( bullet_xpm) );
	perfect = IMG_ReadXPMFromArray( ( char ** ) ( perfect_xpm) );
	round_box = IMG_ReadXPMFromArray( ( char ** ) ( round_box_xpm) );
	game_over_box = IMG_ReadXPMFromArray( ( char ** ) ( game_over_box_xpm) );
	grass_over = IMG_ReadXPMFromArray( ( char ** ) ( grass_over_xpm) );
	fly_away_box = IMG_ReadXPMFromArray( ( char ** ) ( fly_away_xpm) );

    if( menu == NULL ||
		arrow == NULL ||
		duck_bg == NULL ||
		dog_sprites == NULL ||
		dog_sprites == NULL ||
		black_duck == NULL ||
		red_duck == NULL ||
		blue_duck == NULL ||
		pass_lines == NULL ||
		bullet == NULL ||
		perfect == NULL ||
		round_box == NULL ||
		game_over_box == NULL ||
		grass_over == NULL ||
		fly_away_box == NULL)
    {
        cerr << "Errore nel caricamento delle immagini." << endl;
        return false;
    }

    return true;
}

/** Gestisce il caricamento dei suoni del gioco.
 * \return false in caso di errore, true altrimenti.
 */
bool load_sounds()
{
	duck_intro = Mix_LoadMUS( "res/audio/duck_intro.wav" );
	flap = Mix_LoadMUS( "res/audio/flap.wav" );
	menu_sound = Mix_LoadMUS( "res/audio/menu_sound.wav" );

	bark = Mix_LoadWAV( "res/audio/bark.wav" );
	clear = Mix_LoadWAV( "res/audio/clear.wav" );
	duck_get = Mix_LoadWAV( "res/audio/duck_get.wav" );
	game_over_sound = Mix_LoadWAV( "res/audio/game_over.wav" );
	miss = Mix_LoadWAV( "res/audio/miss.wav" );
	perfect_sound = Mix_LoadWAV( "res/audio/perfect.wav" );
	shoot = Mix_LoadWAV( "res/audio/shoot.wav" );
	move_duck_sound = Mix_LoadWAV( "res/audio/move_duck.wav" );
	falling = Mix_LoadWAV( "res/audio/falling.wav" );
	bump = Mix_LoadWAV( "res/audio/bump.wav" );

    if( duck_intro == NULL ||
		flap == NULL ||
		menu_sound == NULL ||
		bark == NULL ||
		clear == NULL ||
		duck_get == NULL ||
		game_over_sound == NULL ||
		miss == NULL ||
		perfect_sound == NULL ||
		shoot == NULL ||
		move_duck_sound == NULL ||
		falling == NULL ||
		bump == NULL )
    {
        cerr << "Errore nel caricamento degli effetti sonori." << endl;
		return false;
    }

    return true;
}

/** Gestisce il caricamento dei font del gioco.
 * \return false in caso di errore, true altrimenti.
 */
bool load_fonts()
{
    font = TTF_OpenFont( "res/font/PressStart.ttf", 16 );
    if( font == NULL )
    {
        return false;
    }

    return true;
}

//******************************************************************************

bool load_files()
{
    if( load_images() == false )
    {
		cerr << "Errore nel caricamento delle immagini." << endl;
		return false;
	}

    if( load_sounds() == false )
    {
		cerr << "Errore nel caricamento dei suoni." << endl;
		return false;
	}

	if( load_fonts() == false )
	{
		cerr << "Errore nel caricamento dei font." << endl;
		return false;
	}

    return true;
}

//******************************************************************************

void clean_files()
{
	SDL_FreeSurface(menu);
	SDL_FreeSurface(arrow);
	SDL_FreeSurface(duck_bg);
	SDL_FreeSurface(dog_sprites);
	SDL_FreeSurface(black_duck);
	SDL_FreeSurface(red_duck);
	SDL_FreeSurface(blue_duck);
	SDL_FreeSurface(pass_lines);
	SDL_FreeSurface(bullet);
	SDL_FreeSurface(perfect);
	SDL_FreeSurface(round_box);
	SDL_FreeSurface(game_over_box);
	SDL_FreeSurface(grass_over);
	SDL_FreeSurface(fly_away_box);

	Mix_FreeMusic(clay_intro);
	Mix_FreeMusic(duck_intro);
	Mix_FreeMusic(flap);
	Mix_FreeMusic(menu_sound);

	Mix_FreeChunk(bark);
	Mix_FreeChunk(clear);
	Mix_FreeChunk(duck_get);
	Mix_FreeChunk(game_over_sound);
	Mix_FreeChunk(miss);
	Mix_FreeChunk(perfect_sound);
	Mix_FreeChunk(shoot);
	Mix_FreeChunk(move_duck_sound);
	Mix_FreeChunk(falling);
	Mix_FreeChunk(bump);
}

//******************************************************************************

void halt_sounds()
{
	Mix_HaltMusic();
	Mix_HaltChannel( -1 );
}

//******************************************************************************

int get_int_digits( int number )
{
	int lengthCount = 0;
	for( ; number != 0; number /= 10, lengthCount++);
	return lengthCount;
}

string convert_score( int score )
{
	stringstream ss;

	if( score > 999999 )
		score %= 1000000;

	ss << setfill('0') << setw(6) << score;

	return ss.str();
}

string convert_round( int round )
{
	stringstream ss;
	ss << round;
	return ss.str();
}

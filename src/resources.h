
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
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

//******************************************************************************

extern SDL_Surface *menu; /**< Immagine del menù. */
extern SDL_Surface *arrow; /**< Cursore del menù. */
extern SDL_Surface *duck_bg; /**< Sfondo del Game A/B. */
extern SDL_Surface *dog_sprites; /**< Spritesheet del cane. */
extern SDL_Surface *blue_duck; /**< Spritesheet della papera blu. */
extern SDL_Surface *black_duck; /**< Spritesheet della papera nera. */
extern SDL_Surface *red_duck; /**< Spritesheet della papera rossa. */
extern SDL_Surface *pass_lines; /**< Linee che indicano il numero minimo di papere. */
extern SDL_Surface *bullet; /**< Immagine del proiettile. */
extern SDL_Surface *perfect; /**< Messaggio 'Perfect'. */
extern SDL_Surface *round_box; /**< Messaggio 'Round'. */
extern SDL_Surface *game_over_box; /**< Messaggio 'Game Over'. */
extern SDL_Surface *grass_over; /**< Immagine parziale dell'erba. */
extern SDL_Surface *fly_away_box; /**< Messaggio 'Fly Away'. */

//******************************************************************************

extern Mix_Music *duck_intro; /**< Musica introduttiva. */
extern Mix_Music *flap; /**< Suono del battito d'ali. */
extern Mix_Music *menu_sound; /**< Musica del menù. */

//******************************************************************************

extern Mix_Chunk *bark; /**< Verso del cane. */
extern Mix_Chunk *clear; /**< Fine del round (vittoria). */
extern Mix_Chunk *duck_get; /**< Presa della papera. */
extern Mix_Chunk *game_over_sound; /**< Game over. */
extern Mix_Chunk *miss; /**< Risata del cane. */
extern Mix_Chunk *perfect_sound; /**< Suono che accompagna il messaggio 'Perfect'. */
extern Mix_Chunk *shoot; /**< Suono dello sparo. */
extern Mix_Chunk *move_duck_sound; /**< Movimento simboli papere. */
extern Mix_Chunk *bump; /**< Tonfo caduta papera. */
extern Mix_Chunk *falling; /**< Suono caduta papera. */

//******************************************************************************

extern TTF_Font *font; /**< Font utilizzato dall'applicazione. */

//******************************************************************************

/** Gestisce il caricamento delle risorse di gioco.
 * \return false in caso di errore, true altrimenti.
 */
bool load_files();

/** Dealloca la memoria occupata dalle risorse del gioco. */
void clean_files();

/** Ferma tutti gli eventi musicali, sia da Mix_PlayChannel che da Mix_PlayMusic. */
void halt_sounds();

/** Imposta o rimuove la modalità fullscreen */
void Check_FullScreen();

//******************************************************************************

/** Restituisce il numero di cifre decimali di un numero.
 * \param number intero da valutare
 * \return numero di cifre decimali del numero.
*/
int get_int_digits( int number );

/** Converte un intero in una stringa, composta da 6 cifre decimali.
 * \return l'intero convertito sotto forma di stringa.
 */
string convert_score( int score );

/** Converte un intero in una stringa.
 * \return l'intero convertito sotto forma di stringa.
 */
string convert_round ( int round );

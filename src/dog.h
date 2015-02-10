
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

/** Definisce i bordi degli sprite del cane. */
void dog_set_clips();

/** Inizializza le variabili dell'animazione introduttiva. */
void dog_init();

/** Muove il cane nella posizione successiva. */
void dog_move();

/** Mostra lo sprite attuale del cane
 * \return true se l'animazione è terminata, false altrimenti.
 */
bool dog_show();

/** Gestisce l'animazione introduttiva
 * \return false in caso di errore
 */
bool dog_intro_animation();

/** Inizializza le variabili dell'animazione della papera colpita. */
void dog_catch_init();

/** Gestisce l'animazione della papera colpita.
 * \param x posizione orizzontale nella quale far apparire il cane.
 * \return false se l'animazione è terminata, false altrimenti.
 */
bool dog_catch_anim( int x );

/** Inizializza le variabili dell'animazione della papera persa. */
void dog_lol_init();

/** Gestisce l'animazione della papera persa.
 * \param back indica se il cane deve tornare sotto la linea dell'erba.
 */
void dog_show_lol( bool back );


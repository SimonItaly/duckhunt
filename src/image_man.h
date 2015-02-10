
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

/** Carica, ottimizza e applica il color key ad un'immagine.
 * \param filename path del file immagine.
 * \return il puntatore alla SDL_Surface contente l'immagine, NULL in caso di errore.
 */
SDL_Surface *load_image( std::string filename );

/** Riempie un'area rettangolare con un dato colore.
 * \param dst SDL_Surface sul quale applicare il colore.
 * \param x origine orizzontale dell'area da colorare.
 * \param y origine verticale dell'area da colorare.
 * \param h altezza dell'area da colorare.
 * \param w larghezza dell'area da colorare.
 * \param color colore dell'area da applicare.
 * \return 0 in caso di successo, un valore negativo in caso di errore.
 */
int SDL_FillRect(SDL_Surface* dst, const int x, const int y, const int h, const int w, Uint32 color);

/** Applica una superficie, o parte di essa, sopra un'altra.
 * \param x origine orizzontale dell'area su cui applicare la superficie.
 * \param y origine verticale dell'area su cui applicare la superficie.
 * \param source superficie da applicare.
 * \param destination superficie su cui applicare la sorgente.
 * \param clip porzione dell'immagine da utilizzare.
 * \return 0 in caso di successo, un valore negativo in caso di errore.
*/
int apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

/** Applica una superficie, o parte di essa, sopra un'altra.
 * \param r area sulla quale applicare la superficie.
 * \param source superficie da applicare.
 * \param destination superficie su cui applicare la sorgente.
 * \param clip porzione dell'immagine da utilizzare.
 * \return 0 in caso di successo, un valore negativo in caso di errore.
*/
int apply_surface( SDL_Rect r, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

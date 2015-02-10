
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

/** Struttura contenente le variabili utilizzate per animare la papera. */
struct duck
{
	int type; /**< colore della papera. */
	int x_offset; /**< posizione orizzontale attuale della papera. */
	int y_offset; /**< posizione verticale attuale della papera. */
	int speed; /**< la velocità con la quale si muove la papera. */
	int angle; /**< l'angolo che definisce la direzione del movimento. */
	int points; /**< punti ottenuti colpendo la papera. */
	int frame; /**< il frame attuale. */
	int counter; /**< contatore dei frame inviati a schermo. */
	bool away; /**< true se la papera è volata via. */
};

/** Definisce i bordi degli sprite della papera. */
void duck_set_clips();

/** Inizializza le variabili dell'animazione.
 * \param d la struttura contenente le variabili della papera.
 * \param speed la velocità con la quale si muove la papera.
 */
void duck_init( duck &d, int speed );

/** Muove il cane nella posizione successiva.
 * \param d la struttura contenente le variabili della papera.
 * \param flyaway gestisce l'animazione al di fuori dello schermo (nel caso la papera venga mancata).
 */
void duck_move( duck &d, bool fly_away);

/** Gestisce l'animazione successiva alla presa della papera.
 * \param d la struttura contenente le variabili della papera.
 */
bool duck_shot_anim( duck &d );

/** Mostra lo sprite attuale della papera.
 * \param d la struttura contenente le variabili della papera.
 */
void duck_show( duck &d );

/** Controlla che un punto si trovi all'interno dello sprite della papera.
 * \param d la struttura contenente le variabili della papera.
 * \param x coordinata orizzontale del punto.
 * \param y la coordinata verticale del punto.
 */
bool duck_range_check( const duck d, int x, int y);

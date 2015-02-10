
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

/** Struttura contenente le variabili utilizzate per la gestione dei timer. */
struct timer
{
	int startTicks; /**< memorizza l'istante (in ms) dell'avvio del timer. */
	int pausedTicks; /**< memorizza l'istante (in ms) della messa in pausa del timer. */
	bool paused; /**< true se il timer è stato messo in pausa, false altrimenti. */
	bool started; /**< true se il timer è stato avviato, false altrimenti. */
};

/** Avvia il timer.
 * \param t timer passato per riferimento.
*/
void timer_start( timer &t );

/** Ferma un timer.
 * \param t timer passato per riferimento.
*/
void timer_stop( timer &t );

/** Mette in pausa un timer.
 * \param t timer passato per riferimento.
*/
void timer_pause( timer &t );

/** Avvia un timer che era stato precedentemente messo in pausa.
 * \param t timer passato per riferimento.
*/
void timer_unpause( timer &t );

/** Ritorna il tempo passato dall'avvio del timer (in ms).
 * \param t timer passato per valore.
 * \return la differenza tra l'istante attuale e l'istante di avvio del timer (in ms).
 */
int timer_get_ticks( timer t );

/** Controlla se un timer è stato avviato.
 * \param t timer passato per valore.
 * \return true se il timer è stato avviato, false altrimenti.
 */
bool timer_is_started( timer t );

/** Controlla se un timer è stato messo in pausa.
 * \param t timer passato per valore.
 * \return true se il timer è stato messo in pausa, false altrimenti.
 */
bool timer_is_paused( timer t );

/** Ritorna il tempo passato dall'avvio del timer (in secondi).
 * \param t timer passato per valore.
 * \return la differenza tra l'istante attuale e l'istante di avvio del timer (in secondi).
 */
int timer_passed_seconds( timer t );

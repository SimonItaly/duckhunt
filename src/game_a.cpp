
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
#include <sstream>
using namespace std;

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_ttf.h"

#include "timer.h"
#include "image_man.h"

#include "game_a.h"

#include "var_init.h"
#include "resources.h"

#include "dog.h"
#include "duck.h"

//******************************************************************************

// Definisce se saltare l'animazione iniziale.
//#define SKIP_DOG_INTRO

/** Valore di ritorno usato da handle_events() per la pressione di ESC. */
#define	RETURN_ESC_EVENT		-1
/** Valore di ritorno usato da handle_events() se non avvengono evento. */
#define	RETURN_NO_EVENT			-2

//******************************************************************************

/** Applica il numero del round.
 * \param round numero del round.
*/
void apply_round ( int round );
/** Applica il messaggio 'Game Over'. */
void apply_game_over();

/** Aggiorna lo schermo corrente, prima di utilizzare SDL_Flip per mandare le immagini a schermo */
void screen_update();

/** Scarta tutti gli eventi in input. */
void halt_events();

/** Gestisce gli eventi per l'uscita dal programma durante le animazioni. */
int handle_events();

/** Visualizza l'animazione del cane dopo la fuga di una papera.
 * \param game_over
 * \return l'evento gestito da handle_events()
*/
int show_duck_miss( bool game_over );

//******************************************************************************

SDL_Surface
    *score_text = NULL, /**< Superficie contente il punteggio attuale. */
    *round_text = NULL, /**< Superficie contentente il round attuale. */
    *round_upper_text = NULL; /**< Superficie contentente il round da applicare con apply_round(). */

int
    bullets_amount = 3, /**< Proiettili rimasti. */
    round = 1, /**< Round attuale. */
    score = 0, /**< Punteggio attuale. */
    event_ret; /**< Intero per la gestione degli eventi tramite handle_events(). */

int
    cur_duck = -1, /**< Papera corrente. */
	hit_ducks,  /**< Papere colpite. */
	pass_ducks = 6;  /**< Papere necessarie per passare il round. */

bool
    hit_duck[11]; /**< Array di papere colpite. */

bool
    black_frame = false, /**< Controlla se mostrare il frame nero dopo uno sparo. */
    duck_caught = false, /**< Controlla se la papera è stata colpita. */
	fly_away = false; /**< Controlla se la papera è volata via. */

timer
    fps, /**< Timer dei frame per second (FPS). */
	round_timer, /**< Timer della scritta 'Round'. */
	duck_blink; /**< Timer che gestisce il flash delle papere durante il gioco. */

//******************************************************************************

int GameA_MainLoop()
{
	int mouse_x,
		mouse_y;

	for( int i = 0; i < 11; i++ )
		hit_duck[i] = false;

	dog_init();

	timer_start( round_timer );

	// ------ Intro Loop Begin ------ //
	#ifndef SKIP_DOG_INTRO
	Mix_PlayMusic( duck_intro, 0 );

	bool intro = true;
	while( intro )
	{
		timer_start( fps );

		event_ret = handle_events();
		if(event_ret != RETURN_NO_EVENT)
			return event_ret;

		intro = dog_intro_animation();
		screen_update();
		if(dog_show())
			apply_surface( 0, 0, grass_over, screen );

		SDL_Flip( screen );

		if( timer_get_ticks( fps ) < 1000 / FRAMES_PER_SECOND )
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer_get_ticks( fps ) );
	}

	SDL_Delay( 2000 );
	#endif
	// ------ Intro Loop End ------ //

	// ------ Game Loop Begin ------ //
	duck d;

	timer fly_away_timer;

	do
	{
		//Reset papere colpite
		cur_duck = 0;
		hit_ducks = 0;

		for( int i = 0; i < 10; i++ )
			hit_duck[i] = false;

		//Loop 10 papere
		for( int i = 0; i < 10; i++ )
		{
			//Suono papera
			Mix_PlayMusic( flap, -1 );

			cur_duck = i;

			bullets_amount = 3;
			duck_caught = false;
			fly_away = false;

			duck_init( d, round );

			timer_start( duck_blink );
			timer_start( fly_away_timer );

			SDL_ShowCursor( 1 );

			while( !duck_caught && !d.away )
			{
				timer_start( fps );

				//Polling degli eventi
				while( SDL_PollEvent( &event ) )
		        {
					switch( event.type )
		            {
						//Uscita dall'applicazione
		                case SDL_QUIT:
		                    return -1;
		                    break;

		                //Handle tasto ESC
						case SDL_KEYDOWN:
		                    switch( event.key.keysym.sym )
		                    {
		                        case SDLK_ESCAPE:
									halt_sounds();
									return 0;
		                            break;

								default:
									break;
							}
							break;

						//Handle click sinistro
						case SDL_MOUSEBUTTONDOWN:
						{
							if( event.button.button == SDL_BUTTON_LEFT )
		        			{
								if(!duck_caught && !fly_away)
								{
									bullets_amount--;

									if(bullets_amount > 0)
									{
										//Frame nero
										black_frame = true;

										//Suono
										Mix_PlayChannel( -1, shoot, 0 );

										//Rimuovi scritta round
										timer_stop( round_timer );

										//Posizione del mouse
										SDL_GetMouseState(&mouse_x, &mouse_y);

										//Controllo posizione papera
										if(duck_range_check(d, mouse_x, mouse_y))
										{
											duck_caught = true;

											//Update punteggio
											score += d.points;
									    	score_text = TTF_RenderText_Solid( font, ( convert_score( score ) ).c_str(), white_font );

									    	cur_duck = -1;

									    	hit_duck[i] = true;
									    	hit_ducks++;
										}
									}
									else
									{
										fly_away = true;
										d.angle = 270;
									}
								}
							}
							break;
						}

						//Altro
						default:
							break;
					}

				}

				//Fly away
				if( timer_passed_seconds( fly_away_timer ) >= 5 )
				{
					timer_stop( fly_away_timer );
					fly_away = true;
					d.angle = 270;
				}

				screen_update();
				duck_move( d, fly_away );
				duck_show( d );

				SDL_Flip( screen );

				if( timer_get_ticks( fps ) < ( 1000 / FRAMES_PER_SECOND ) )
					SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer_get_ticks( fps ) );
			}

			Mix_HaltMusic();

			fly_away = false;
			cur_duck = -1;

			//Se papera colpita -> animazione caduta
			if(duck_caught)
			{
				//Suono caduta
				Mix_PlayChannel( -1, falling, 0 );

				bool animation;

				//Animazione caduta papera
				do
				{
					event_ret = handle_events();
					if(event_ret != RETURN_NO_EVENT)
						return event_ret;

					screen_update();

					animation = duck_shot_anim( d );

					SDL_Flip( screen );

					SDL_Delay( 5 );
				}
				while(animation);

				halt_sounds();

				//Suono caduta
				Mix_PlayChannel( -1, bump, 0 );

				SDL_Delay( 1000 );

				//Suono papera presa
				Mix_PlayChannel( -1, duck_get, 0 );

				//Animazione presa cane
				dog_catch_init();
				do
				{
					event_ret = handle_events();
					if(event_ret != RETURN_NO_EVENT)
						return event_ret;

					screen_update();

					animation = dog_catch_anim( d.x_offset );

					SDL_Flip( screen );

					SDL_Delay( 5 );
				}
				while(animation);
			}
			else
			{
				SDL_Delay( 1000 );
				Mix_PlayChannel( -1, miss, 0 );

				show_duck_miss( false );
			}

			halt_events();

			timer_stop( fly_away_timer );
			timer_stop( duck_blink );
		}

		halt_sounds();

		//Almeno una e non tutte le papere colpite
		if(hit_ducks > 0 && hit_ducks < 10)
		{
			int blank_pivot = 0,
				hit_pivot = 0;

			//Spostamento papere verso sinistra
			do
			{
				while( blank_pivot != hit_pivot )
				{
					timer_start( fps );

					event_ret = handle_events();
					if(event_ret != RETURN_NO_EVENT)
						return event_ret;

					for(int i = hit_pivot; i < 10; i++)
					{
						hit_duck[ i-1 ] = hit_duck[ i ];
						hit_duck[ i ] = hit_duck[ i+1 ];
					}
					hit_pivot--;

					Mix_PlayChannel( -1, move_duck_sound, 0 );

					screen_update();

					SDL_Flip( screen );

					SDL_Delay( 400 );
				}

				for(int i = 0; i < 10; i++)
				{
					if(!hit_duck[i])
					{
						hit_pivot = blank_pivot = i;
						for(int j = blank_pivot + 1; j < 10; j++)
						{
							if(hit_duck[j])
							{
								hit_pivot = j;
								break;
							}
						}
						break;
					}
				}
			}
			while( blank_pivot != hit_pivot );
		}

		//Se vinto flasha le papere sette volte
		if(hit_ducks >= pass_ducks)
		{
			//Copia temporanea delle hit ducks
			bool temp_hit_duck[10];
			memcpy(temp_hit_duck, hit_duck, sizeof(bool) * 10);

			int flashings = 0;

			timer flash_ducks;
			timer_start( flash_ducks );

			//Musichetta flash papere
			Mix_PlayChannel( -1, clear, 0 );

			//Flash loop
			while( flashings < 7 )
			{
				event_ret = handle_events();
				if(event_ret != RETURN_NO_EVENT)
					return event_ret;

				screen_update();

				if( ( timer_get_ticks( flash_ducks ) / 400 ) % 2 )
				{
					memset(hit_duck, 0, sizeof(bool) * 10);
				}
				else
				{
					memcpy(hit_duck, temp_hit_duck, sizeof(bool) * 10);
					flashings++;
				}

				SDL_Flip( screen );

				SDL_Delay( 400 );
			}

			timer_stop( flash_ducks );

			//Se perfetto mostra scritta perfetto + musica perfetto + delay
			if(hit_ducks == 10)
			{
				halt_sounds();

				score += 10000;
		    	score_text = TTF_RenderText_Solid( font, ( convert_score( score ) ).c_str(), white_font );

		    	screen_update();

		    	apply_surface( 183, 86, perfect, screen );

		    	SDL_Flip( screen );

		    	Mix_PlayChannel( -1, perfect_sound, 0 );
		    	SDL_Delay( 4000 );
			}

			//Round successivo
			round++;
			bullets_amount = 3;

			timer_start( round_timer );

			//Se round > 10 aumenta le pass_ducks.
			if( round % 10 == 0 && pass_ducks < 10 )
				pass_ducks++;
		}

		SDL_ShowCursor( 0 );
	}
	while( hit_ducks >= pass_ducks );
	// ------ Game Loop End ------ //

	// ------ Game Over Loop Begin ------ //
	Mix_PlayChannel( -1, game_over_sound, 0 );

	event_ret = show_duck_miss( true );
	if(event_ret != RETURN_NO_EVENT)
		return event_ret;

	// ------ Game Over Loop End ------ //

	return score;
}

//****************************************************************************//

int show_duck_miss( bool game_over )
{
	timer game_over_timer;
	timer_start( game_over_timer );

	int timer_seconds,
		max_seconds = 4;

	if(game_over)
		max_seconds += 3;

	dog_lol_init();

	//Animazione cane con game over
	do
	{
		//Polling degli eventi
		event_ret = handle_events();
		if(event_ret != RETURN_NO_EVENT)
			return event_ret;

		screen_update();

		timer_seconds = timer_passed_seconds( game_over_timer );

		if(game_over)
		{
			apply_game_over();
			if( timer_seconds > 2 )
			{
				dog_show_lol( false );
			}
		}
		else
		{
			dog_show_lol( timer_seconds > 2 );
		}

		apply_surface( 0, 0, grass_over, screen );

		SDL_Flip( screen );
	}
	while(timer_seconds < max_seconds);
	
	return RETURN_NO_EVENT;
}

//****************************************************************************//

void apply_round( int round )
{
	apply_surface( 207, 86, round_box, screen );

	round_upper_text = TTF_RenderText_Solid( font, ( convert_round( round ) ).c_str(), white_font );
	apply_surface( 250 - 9 * (get_int_digits(round) - 1), 127, round_upper_text, screen );
}

void apply_game_over()
{
	apply_surface( ( ( SCREEN_WIDTH / 2 ) - ( game_over_box->w / 2 ) ), 86, game_over_box, screen );
}

void screen_update()
{
	SDL_Rect round_rect = { 96, 352, 0, 16 };
	SDL_Rect duck_rect = { 0, 388, 14, 14 };
	SDL_Rect pass_rect = { 0, 0, 16, 14 };

	if( !fly_away )
		//Sfondo blu
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x3F, 0xBF, 0xFF ) );
	else
		//Sfondo rosa
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xBF, 0xB3 ) );

	//Icona papera bianca o rossa
	for(int i = 0; i < 10; i++)
	{
		duck_rect.x = 192 + 16 * i;

		//Se la papera è stata colpita l'icona è rossa
		if(hit_duck[i])
			SDL_FillRect( screen, &duck_rect, SDL_MapRGB( screen->format, 228, 68, 55 ) );
		else
		{
			//Flash bianco-nero della papera gestito dal timer
			if(i == cur_duck)
			{
				if( ( timer_get_ticks( duck_blink ) / 150 ) % 2 )
					SDL_FillRect( screen, &duck_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
				else
					SDL_FillRect( screen, &duck_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );
			}
			else
				SDL_FillRect( screen, &duck_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
		}
	}

	//Papere per passare il round
	for(int i = 0; i < pass_ducks; i++)
	{
		apply_surface( 192 + (16 * i), 402, pass_lines, screen, &pass_rect);
	}
	pass_rect.x = 16;
	for(int i = pass_ducks; i < 10; i++)
	{
		apply_surface( 192 + (16 * i), 402, pass_lines, screen, &pass_rect );
	}

	//Interfaccia
	apply_surface( 0, 0, duck_bg, screen );

	//Proiettili rimanenti
	for(int i = 0; i < bullets_amount; i++)
	{
		apply_surface( 52 + 16 * i, 384, bullet, screen );
	}
	//Proiettili sparati (sprite nero)
	for(int i = bullets_amount; i < 3; i++)
	{
		SDL_FillRect( screen, 52 + 16 * i, 384, 14, 8, SDL_MapRGB( screen->format, 0, 0, 0 ) );
	}

	//Punteggio
	score_text = TTF_RenderText_Solid( font, ( convert_score( score ) ).c_str(), white_font );
	apply_surface( 383, 384, score_text, screen );

	//Background round se maggiore di 9
	if( round > 9 )
	{
		round_rect.w = 16 * ( get_int_digits( round ) - 1);
		SDL_FillRect( screen, &round_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
	}

	//Testo round
	round_text = TTF_RenderText_Solid( font, ( convert_round( round ) ).c_str(), green_font );
	apply_surface( 80, 353, round_text, screen );

	//Msgbox round
	if(timer_is_started( round_timer ) && timer_passed_seconds( round_timer ) < 3)
	{
		apply_round(round);
	}
	else
	{
		timer_stop( round_timer );
	}

	if(fly_away)
	{
		//Box "Fly Away"
		apply_surface( 183, 86, fly_away_box, screen );
		//SDL_Flip( screen );
	}
	else if(black_frame)
	{
		//Schermo nero per un frame
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );
		black_frame = false;

		SDL_Flip( screen );
	}
}

void halt_events()
{
	while(SDL_PollEvent(&event) != 0);
}

int handle_events()
{
	if( SDL_PollEvent( &event ) )
    {
		switch( event.type )
        {
			//Uscita dall'applicazione
            case SDL_QUIT:
                return RETURN_ESC_EVENT;
                break;

            //Handle tasto ESC
			case SDL_KEYDOWN:
                switch( event.key.keysym.sym )
                {
                    case SDLK_ESCAPE:
						halt_sounds();
						return score;
                        break;

					default:
						break;
				}
				break;

			//Altro
			default:
				break;
		}
	}
	return RETURN_NO_EVENT;
}


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

/**
  * @mainpage Documentazione di Duck Hunt
  * ### Presentazione
  * Remake del popolare gioco per Nintendo Entertainment System. \n
  * Il programma è stato scritto in C++ utilizzando la libreria <a href="https://www.libsdl.org/index.php">Simple DirectMedia Library (SDL) 1.2</a>. \n
  * Le immagini ed i file audio sono stati estratti dal gioco originale e sono da considerarsi di proprietà di <i>Nintendo Co., Ltd</i>.
  
  * ### Struttura del programma
  * Il programma si compone dei seguenti moduli:
  * <ul>
  * 	<li><b>dog.h</b>: gestisce le animazioni del cane classico di Duck Hunt (intro, cattura, risata).</li>
  * 	<li><b>duck.h</b>: gestisce le animazioni della papera ed il suo movimento.</li>
  * 	<li><b>game_a.h</b>: gestisce il primo gioco ("Game A - One Duck"), con un livello di difficoltà crescente (velocità della papera e numero minimo di papere da colpire) in base al round corrente.</li>
  * 	<li><b>game_init.h</b>: richiama le funzioni di caricamento delle risorse e perl'avvio di SDL.</li>
  * 	<li><b>image_man.h</b>: contiene le funzioni di caricamento ed ottimizzazione delle immagini e per la loro applicazione sullo schermo.</li>
  * 	<li><b>resources.h</b>: gestisce le variabili delle risorse, le loro funzioni di caricamento ed alcune funzioni di conversione di tipo di dato.</li>
  * 	<li><b>timer.h</b>: gestisce la struttura e le funzioni per la creazione e l'utilizzo di timer.</li>
  * 	<li><b>var_init.h</b>: contiene i riferimenti alle variabili ed alle costanti condivise da tutti i file (e.g. dimensione dello schermo).</li>
  * </ul>
  
  * ### Introduzione
  * Per iniziare la navigazione nel codice si può visualizzare il file main.cpp, il modulo dal quale inizia l'esecuzione del programma.
  
  * <hr>
  
  * @author Simone Bisi
  * @author Andrea Carpino
  */
  
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"

#include "timer.h"
#include "image_man.h"

#include "game_a.h"

#include "var_init.h"
#include "game_init.h"
#include "resources.h"

//******************************************************************************

/** Path di salvataggio del top score su file binario. */
#define SCORE_SAVE_PATH			"res/save/topscore"

SDL_Rect arrow_position[3]; /**< Array contenente le posizioni assunte dalla freccia all'interno del menù. */
int arrow_now = 0; /**< Definisce la posizione attuale della freccia del menù. */

/** Definisce le coordinate delle posizioni assunte dalla freccia nel munù. */
void set_arrow_position()
{
    arrow_position[0].x = 96;
    arrow_position[0].y = 258;
    arrow_position[1].x = 96;
    arrow_position[1].y = 290;
    arrow_position[2].x = 96;
    arrow_position[2].y = 322;
}

//******************************************************************************

int main( int argc, char* args[] )
{
    //Flag di quit
    bool quit = false;
	
	//Top score
	int top_score = 0;
	SDL_Surface *score_text;
	fstream score_file;
	
    //Init di SDL e risorse
    if( init() == false )
    {
        cerr << "Errore nel richiamo della funzione init()." << endl;
		return 1;
    }

    //Caricamento files
    if( load_files() == false )
    {
        cerr << "Errore nel richiamo della funzione load_files()." << endl;
		return 1;
    }

	//Lettura del top-score da file binario
	score_file.open(SCORE_SAVE_PATH, ios::in | ios::binary);
    if(score_file)
    {
		score_file.read(reinterpret_cast<char *>(&top_score), sizeof(top_score));
		score_file.close();
	}

	//Posizione iniziale della freccia
    set_arrow_position();

    //Surface top-score
    score_text = TTF_RenderText_Solid( font, ( convert_score( top_score ) ).c_str(), green_font );

    //Timer per regolare i FPS
    timer fps;

    //Musica del menù, ripetuta ogni 30 secondi circa
    Mix_PlayMusic( menu_sound, -1 );

    while( quit == false )
    {
        timer_start( fps );

        ////Polling degli eventi
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
				//Uscita dall'applicazione
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                {
                    switch( event.key.keysym.sym )
                    {
						//Handle tasto ESC
                        case SDLK_ESCAPE:
                        {
                            quit = true;
                            break;
                        }

                        //Handle freccia su
                        case SDLK_UP:
                        {
							Mix_HaltMusic();
                            Mix_PlayChannel( -1, shoot, 0 );

                            if( --arrow_now < 0 )
                                arrow_now = 2;
                            break;
                        }

                        //Handle freccia giù
                        case SDLK_DOWN:
                        {
                            Mix_HaltMusic();
                            Mix_PlayChannel( -1, shoot, 0 );

                            if( ++arrow_now > 2 )
                                arrow_now = 0;
                            break;
                        }

						case SDLK_F11:
						{
							Check_FullScreen();
							break;
						}

                        //Handle tasto invio
                        case SDLK_KP_ENTER:
                        case SDLK_RETURN:
                        {
							Mix_HaltMusic();

							//Menu
							switch(arrow_now)
							{
								//Game A
								case 0:
								{
									Mix_HaltMusic();
									int ret_score = GameA_MainLoop();

									if( ret_score < 0 )
									{
										//Uscita dall'applicazione
										quit = true;
									}
									else
									{
										//Game over o pressione di ESC
										if(ret_score > top_score)
										{
											top_score = ret_score;
										}
										Mix_PlayMusic( menu_sound, -1 );

								    	score_text = TTF_RenderText_Solid( font, ( convert_score( top_score ) ).c_str(), green_font );
									}
									break;
								}

								//Game B, two ducks (non previsto in questo progetto)
								case 1:
									//GameB_MainLoop();
									break;

								//Game C, clay shooting (non previsto in questo progetto)
								case 2:
									//GameC_MainLoop();
									break;

								default:
									break;
							}
                        	break;
                        }

                        //Altro
                        default:
                        	break;
                    }
                }
                default:
                    break;
            }
        }

		//Immagine menù
		apply_surface( 0, 0, menu, screen );

		//Freccia
		apply_surface( arrow_position[ arrow_now ], arrow, screen );

		//Top-score
		apply_surface( 304, 368, score_text, screen );

		//Update schermo
        SDL_Flip( screen );

        //FPS
        if( timer_get_ticks( fps ) < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer_get_ticks( fps ) );
        }
    }

    //Scrittura del top-score su file binario
    score_file.open(SCORE_SAVE_PATH, ios::out | ios::binary);
	if(score_file)
    {
    	score_file.write(reinterpret_cast<const char *>(&top_score), sizeof(top_score));
		score_file.close();
	}

    //Clean up
	clean_files();

    //Termina SDL_Mixer
    Mix_CloseAudio();

    //Termina SDL
    SDL_Quit();

    return 0;
}


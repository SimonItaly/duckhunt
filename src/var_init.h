
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
using namespace std;

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

//******************************************************************************

/** Larghezza della finestra. */
const int SCREEN_WIDTH = 512;

/** Altezza della finestra. */
const int SCREEN_HEIGHT = 448;

/** Altezza minima raggiunta da una papera in volo. */
const int GRASS_LINE = 220;

/** Valore dei bit per pixel. */
const int SCREEN_BPP = 32;

/** Frame per second (FPS). */
const int FRAMES_PER_SECOND = 50;

/** Colore verde per i font. */
const SDL_Color green_font = { 131, 211, 19 };
/** Colore bianco per i font. */
const SDL_Color white_font = { 255, 255, 255 };

/** Surface principale. */
extern SDL_Surface *screen;

/** Struct per gestire gli eventi. */
extern SDL_Event event;

/** Variabile per il cursore utilizzato da SDL. */
extern SDL_Cursor* cursor;

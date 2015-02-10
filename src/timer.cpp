
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

#include "timer.h"

//******************************************************************************

void timer_start( timer &t )
{
	t.started = true;
	t.paused = false;

	t.startTicks = SDL_GetTicks();
	t.pausedTicks = 0;
}

void timer_stop( timer &t )
{
	t.started = false;
	t.paused = false;
}

void timer_pause( timer &t )
{
	if( ( t.started == true ) && ( t.paused == false ) )
	{
		t.paused = true;
		t.pausedTicks = SDL_GetTicks() - t.startTicks;
	}
}

void timer_unpause( timer &t )
{
	if( t.paused == true )
	{
		t.paused = false;
		t.startTicks = SDL_GetTicks() - t.pausedTicks;
		t.pausedTicks = 0;
	}
}

int timer_get_ticks( timer t )
{
	if( t.started == true )
	{
		if( t.paused == true )
		{
			return t.pausedTicks;
		}
		else
		{
			return SDL_GetTicks() - t.startTicks;
		}
	}
	return 0;
}

bool timer_is_started( timer t )
{
	return t.started;
}

bool timer_is_paused( timer t )
{
	return t.paused;
}

int timer_passed_seconds( timer t )
{
	return timer_get_ticks( t ) / 1000;
}

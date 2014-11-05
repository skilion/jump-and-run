#pragma once
#ifndef __CONST_H__
#define __CONST_H__


#include <Math/Vector.h>

using namespace myengine;


//#define NO_BLOOD

const int	TEX_SIZE_PIXEL	= 512;
const int	SCREEN_X_PIXEL	= 640;
const int	SCREEN_Y_PIXEL	= 480;
const int	TILE_SIZE_PIXEL	= 32;

const Vec2i	SCREEN_MIN(0, 0);
const Vec2i	SCREEN_MAX(SCREEN_X_PIXEL, SCREEN_Y_PIXEL);

const char * const CREDITS[] = {
	"August-September 2011",
	"",
	"",
	"-   Programming & Graphics   -",
	"Andrea R.",
	"",
	"-   Musics used   -",
	"",
	"\"The Big House\"",
	"from the album:",
	"\"Audionautix: Tech, Urban, Dance\"",
	"by Jason Shaw",
	"",
	"\"Yellow Dust\"",
	"\"Leafless Quince Tree\"",
	"from the album:",
	"\"Straw Fields\"",
	"by Rolemusic",
	"",
	"\"You've Only Yourself To Blame For Your Tragic Loss In Faith\"",
	"\"We Need Cranes, Not Skyhooks\"",
	"\"Inside (Reprise)\"",
	"from the album:",
	"\"Utter Mess\"",
	"by Edward Shallow",
	"",
	"",
	"",
	"",
	0
};


#endif //!__CONST_H__
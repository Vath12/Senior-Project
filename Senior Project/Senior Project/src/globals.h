#pragma once
#include "SDL.h"

struct input {
	bool key[512];
	bool press[512];
	bool release[512];
};

const double tile_halfwidth = 0.5;
const double tile_halfheight = 0.25;

extern input keys;
extern double deltaTime;
extern int window_width;
extern int window_height;
extern int window_size;
extern int mouse_x;
extern int mouse_y;
extern Uint32 mouse_state;
extern Uint32 mouse_state_single;

extern double camera_x;
extern double camera_y;
extern double camera_viewportWidth;
extern SDL_Window* window;

const double max_viewportWidth = 4096;
const double min_viewportWidth = 50;


/*
direction codes:
	0:North
	1:North East
	2:East
	3:South East
	4:South
	5:South West
	6:West
	7:North West
*/

enum compass {
	N = 0,
	NE = 1,
	E = 2,
	SE = 3,
	S = 4,
	SW = 5,
	W = 6,
	NW = 7
};
#pragma once

struct input {
	bool key[512];
	bool press[512];
	bool release[512];
};

extern input keys;
extern double deltaTime;
extern int window_width;
extern int window_height;
extern int window_size;

extern double camera_x;
extern double camera_y;
extern double camera_viewportWidth;

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
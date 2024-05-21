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
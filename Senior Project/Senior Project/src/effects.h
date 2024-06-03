#pragma once
#include <SDL.h>
#include "vector2.h"
#include <vector>
#include "sprite.h"

struct bulletTracer {
	vector2 from;
	double distance=100;
	vector2 direction;
	double speed=120;
	double length=0.4;
	double time=0;
};

struct corpse {
	sprite* img;
	vector2 position;
	int direction;
	int z;
	int frame;
	int numFrames;
	double time;
	double framerate;
};
void updateCorpses(double deltaTime);
void drawCorpses(SDL_Renderer* r);
void newCorpse(sprite* corpse,vector2 position,int direction,int z = 0, int frames=0, double framerate = 0);
void newBulletFX(vector2 muzzle,vector2 target);
void newBulletFX(vector2 muzzle, vector2 direction, double distance);
void drawFX(SDL_Renderer* renderer, double deltaTime);
void freeFX();



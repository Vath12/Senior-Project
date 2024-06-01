#pragma once
#include <SDL.h>
#include "vector2.h"
#include <vector>

struct bulletTracer {
	vector2 from;
	double distance=100;
	vector2 direction;
	double speed=120;
	double length=0.2;
	double time=0;
};


void newBulletFX(vector2 muzzle,vector2 target);
void drawFX(SDL_Renderer* renderer, double deltaTime);
void freeFX();



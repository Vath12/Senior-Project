#include "effects.h"
#include <SDL.h>
#include "vector2.h"
#include <vector>
#include "render.h"
#include <iostream>
#include <algorithm>
#include "sprite.h"

std::vector<bulletTracer> tracers = std::vector<bulletTracer>();
std::vector<corpse> corpses = std::vector<corpse>();


void newCorpse(sprite* img, vector2 position, int direction, int z, int frames, double framerate) {
	corpses.push_back(corpse(img,position,direction,z,0,frames,0.0,framerate));
}

void newBulletFX(vector2 muzzle, vector2 target) {
	vector2 d = (target - muzzle);
	tracers.push_back({muzzle,d.getMagnitude(),d.normalized()});
}

void newBulletFX(vector2 muzzle, vector2 direction, double distance) {
	tracers.push_back({muzzle,distance,direction});
}


void updateCorpses(double deltaTime) {
	for (int i = 0; i < corpses.size();i++){
		corpse* c = &corpses[i];
		if (c->framerate > 0 && c->frame < c->numFrames-1) {
			if (c->time <= 0) {
				c->frame++;
				c->time = 1.0 / c->framerate;
			}
			c->time -= deltaTime;
		}
	}
}

void drawCorpses(SDL_Renderer* r) {

	for (corpse c : corpses) {

		c.img->draw(r,c.position,c.frame,c.direction,c.z);
	}
}

void drawFX(SDL_Renderer* renderer, double deltaTime) {
	
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

	for (int i = 0; i < tracers.size(); i++) {
		bulletTracer* t = &tracers[i];

		vector2 b = t->from + t->direction * t->speed * t->time;
		vector2 a = b - t->direction * t->length;

		t->time += deltaTime;


		a = worldToCameraIso(a);
		b = worldToCameraIso(b);
		
		double progress = 1-((a - t->from).getMagnitude()/t->distance);

		SDL_SetRenderDrawColor(renderer,255,255,0, (1.0-(4.0*pow(progress,2.0))) * 255);
		SDL_RenderDrawLine(renderer,a.x,a.y,b.x,b.y);

		if ( (t->speed * t->time) > t->distance) {
			tracers[i] = tracers.back();
			tracers.pop_back();
			i -= 1;
		}
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
void freeFX() {

}
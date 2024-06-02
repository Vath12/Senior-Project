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

bool compareCorpse(corpse A, corpse B) {
	return worldToCameraIso(A.position).y < worldToCameraIso(B.position).y;
}

void newCorpse(sprite* img, vector2 position, int direction) {
	corpses.push_back(corpse(img,position,direction));
}

void newBulletFX(vector2 muzzle, vector2 target) {
	vector2 d = (target - muzzle);
	tracers.push_back({muzzle,d.getMagnitude(),d.normalized()});
}

void drawCorpses(SDL_Renderer* r) {
	std::sort(corpses.begin(), corpses.end(), compareCorpse);
	for (corpse c : corpses) {
		c.img->x = c.position.x;
		c.img->y = c.position.y;
		c.img->draw(r,0,c.direction);
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
		
		double progress = (a - t->from).getMagnitude() / t->distance;

		SDL_SetRenderDrawColor(renderer,255,255,0, progress * 255);
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
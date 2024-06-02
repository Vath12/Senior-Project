#pragma once
#include <SDL.h>
#include <vector>
#include "vector2.h"

class sprite {

	public:

		static void init();

		double centerX=0.5;
		double centerY=0.5;

		double w = 0;
		double h = 0;

		double min_bound_x=0;
		double min_bound_y=0;
		double max_bound_x=1;
		double max_bound_y=1;

		int cellSize=0;

		SDL_Texture* texture;
		sprite();
		sprite(SDL_Texture* texture, double w, double h);
		sprite(SDL_Texture* texture, double w, double h,int cellSize);

		void draw(SDL_Renderer* renderer,vector2 position,int srcX,int srcY, int z);
};

struct drawData {
	sprite* img;
	SDL_Rect rect;
	int z;
	unsigned int srcX;
	unsigned int srcY;
};

void drawSprites(SDL_Renderer* renderer);
 
extern std::vector<drawData> drawQueue;
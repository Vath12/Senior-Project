#pragma once
#include <SDL.h>
#include <vector>
class sprite {

	public:

		static void init();

		double x = 0;
		double y = 0;
		double w = 0;
		double h = 0;
		int cellSize=0;

		SDL_Texture* texture;
		sprite();
		sprite(SDL_Texture* texture,double x, double y, double w, double h);
		sprite(SDL_Texture* texture, double x, double y, double w, double h,int cellSize);

		void draw(SDL_Renderer* renderer);
		void draw(SDL_Renderer* renderer,int srcX,int srcY);
};

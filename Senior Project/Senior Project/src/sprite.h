#pragma once
#include <SDL.h>
#include <vector>
class sprite {

	public:

		static void init();
		static void update(SDL_Renderer* renderer, double deltaTime);

		double x = 0;
		double y = 0;
		double w = 0;
		double h = 0;
		bool isSpriteSheet=true;
		int cellSize=0;
		int frames=0;
		int numAnims=0;

		double frameTimer = 0;
		int currentFrame = 0;
		int currentRow = 0;
		int numFrames = 0;
		int frameRate = 0;
		bool playing = false;

		SDL_Texture* texture;
		sprite();
		sprite(SDL_Texture* texture,double x, double y, double w, double h);
		sprite(SDL_Texture* texture, double x, double y, double w, double h,int cellSize);

		void playAnimation(int row, int numFrames, int framerate);
		void updateAnimation(double deltaTime);
		void draw(SDL_Renderer* renderer);
};

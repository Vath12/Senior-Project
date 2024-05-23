#pragma once
#include <SDL.h>
#include <vector>
#include "vector2.h"
#include "sprite.h"

struct animationState {
	int animID=0;
	int numFrames=0;
	int frame=0;
	int rate=0;
	double timer = 0;
};

class entity {

	public:

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
		int direction;
		vector2 position;
		sprite* mainSprite;
		animationState animation;
		bool animating = false;

		entity();
		void update(double deltaTime);
		void draw(SDL_Renderer* renderer);
		void playAnimation(int animID,int numFrames,int startingFrame,int rate);


		static entity* create(vector2 _position, int _direction, sprite* mainSprite);
		static void destroy(entity* e);
		static void freeAll();
		static entity* getEntity(int at);
		static int numEntities();
};
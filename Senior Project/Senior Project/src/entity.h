#pragma once
#include <SDL.h>
#include <vector>
#include "vector2.h"
#include "sprite.h"

struct animationState {

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

		entity();
		void update(double deltaTime);
		void draw(SDL_Renderer* renderer);
		
		static entity* create(vector2 _position, int _direction, sprite* mainSprite);
		static void destroy(entity* e);
		static void freeAll();
		static entity* getEntity(int at);
		static int numEntities();
};
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

enum ENTITYFLAGS {
	selected = 0,
	burning = 1,
	bleeding = 2
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
		animationState* animation;
		bool animating = false;
		vector2 hitbox;
		vector2 hitboxCenter;
		double zPos=0;
		unsigned long int flags = 0;

		void setFlag(unsigned long int id,bool state);
		bool getFlag(unsigned long int id);

		entity(sprite* _mainSprite,vector2 position,int direction);
		virtual void update(double deltaTime);
		void draw(SDL_Renderer* renderer);
		virtual void start();
		virtual void onDestroy();
		static bool compareEntity(entity* A,entity* B);
};
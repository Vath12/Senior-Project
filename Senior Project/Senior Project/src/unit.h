#pragma once
#include "SDL.h"
#include "entity.h"
#include "sprite.h"
#include "vector2.h"

class unit : public entity
{
	public:
		/*
		vp (vitality points) : reaches 0, unit dies (life functioning)
		mp (mobility points) : mobility multiplier
		op (offense points) : reaches 0, unit cannot attack (offensive weapon hp)
		ap (armor points) : unit's defense multiplier
		fp (fear points) : governs attack frequency, willingness to flee etc. (unit's suppression level)
		sp (stamina points) : consumed while moving at dash speed
		ammunition : reaches 0, unit cannot attack
		fuel : reaches 0, unit cannot move or attack (equivalent to food for humans)
		*/
		double vp = 0;
		double mp = 0;
		double op = 0;
		double ap = 0;
		double fp = 0;
		double sp = 0;
		int ammunition = 0;
		int fuel = 0;

		
		double dash=0;
		double speed=0;
		double acceleration=0;

		vector2 velocity;

		vector2 destination;
		bool moving = false;

		animationState moveState;
		animationState idleState;

		sprite* moveAnimation;
		sprite* idleAnimation;

		unit(sprite* _mainSprite, vector2 _position, int _direction);

		void update(double deltaTime);



};


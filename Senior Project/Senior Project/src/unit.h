#pragma once
#include "SDL.h"
#include "entity.h"
#include "sprite.h"
#include "vector2.h"

class group;

enum RANKS {
	Private = 0,
	PrivateFirstClass = 1,
	Corporal = 2,
	Specialist = 4,
	Sergeant=5,
	SecondLiuetenant=6,
	FirstLiuetenant=7,
	Captain=8,
	Major=9
};

struct weapon {
	double bluntDamage = 0;
	double penetration = 0;
	double postPenetrationDamage = 0;
	double range;
	//hit targets at max range
	double accuracy;
	//hit moving targets at close range
	double handling;
	//hit moving targets at long range
	double leading;
	double fireRate;
	int magazineSize;
	double reloadTime;
	double movementSpeedPenalty;
	double movementAccuracyPenalty;
};

bool willHit(weapon* w, double range, double lateralMovement, bool moving);

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
		double vp = 1;
		double mp = 0;
		double op = 0;
		double ap = 0;
		double fp = 0;
		double sp = 0;
		int ammunition = 0;
		int fuel = 0;

		group* parent;
		int rank = 0;

		double dash=0;
		double speed=0;
		double acceleration=0;

		vector2 velocity;

		vector2 relativePosition;

		vector2 destination;
		bool moving = false;
		bool attacking = false;

		animationState moveState;
		animationState idleState;
		animationState fireState;

		sprite* moveAnimation;
		sprite* idleAnimation;
		sprite* fireAnimation;

		weapon* armament = nullptr;
		double attackCooldown;
		int magazine;

		unit(sprite* _mainSprite, vector2 _position, int _direction);

		void moveTo(vector2 destination);

		void update(double deltaTime);

		void debugDraw(SDL_Renderer* renderer);

		bool attackReady();

		void attack(unit* target);

		void takeDamage(weapon* source);
};


#pragma once
#include "SDL.h"
#include "entity.h"
#include "unit.h"
#include "sprite.h"
#include "vector2.h"
#include "quadtree.h"
#include "combat.h"

class building : public entity
{
	public:

		double structure=1;

		std::vector<WeaponData> garrison;
		std::vector<arcOfFire> fieldsOfFire;
		std::vector<unit*> targets;


		int team = 0;
		double maxRange = 0;
	
		bool fired = false;
		bool firing = false;

		sprite* idle;
		animationState idleState;
		sprite* fire;
		animationState fireState;
		sprite* dead;

		building(sprite* _mainSprite, vector2 _position, int _direction);

		void attack(int member, unit* target);

		void update(double deltaTime);
		void findTargets(quadtree* tree);
		void addToGarrison(weapon* w, double arcAngle,double arc);

};

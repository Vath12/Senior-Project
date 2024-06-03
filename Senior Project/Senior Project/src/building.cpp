#include "building.h"
#include "combat.h"
#include "SDL.h"
#include "entity.h"
#include "sprite.h"
#include "vector2.h"
#include "quadtree.h"
#include "effects.h"
#include <iostream>
#include "util.h"

building::building(sprite* _mainSprite, vector2 _position, int _direction) :
	entity(_mainSprite, _position, _direction) {
	garrison = std::vector<WeaponData>();
	fieldsOfFire = std::vector<arcOfFire>();
	targets = std::vector<unit*>();
	animating = true;
}

void building::update(double deltaTime) {

	for (int i = 0; i < garrison.size();i++) {
		if (targets[i] != nullptr && garrison[i].fireCooldown <= 0) {
			firing = true;
			fired = false;
			mainSprite = fire;
			animation = &fireState;

			attack(i,targets[i]);
		}
		garrison[i].fireCooldown -= deltaTime;
	}

	if (firing) {
		if (fireState.frame == fireState.numFrames-1) {
			fired = true;
		}
		if (fireState.frame == 0 && fired) {
			firing = false;
		}
	}
	if (!firing){
		mainSprite = idle;
		animation = &idleState;
	}
	animation->animID = direction;

	entity::update(deltaTime);

}

void building::attack(int member, unit* target) {
	if (inFireArc(position, target->position, fieldsOfFire[member].direction, fieldsOfFire[member].arc)) {
		//std::cout << position.x << " " << position.y << " " << target->position.x << " " << target->position.y << std::endl;
		bool hit = willHit(garrison[member].type, position, target->position, target->velocity,false);

		if (hit) {
			newBulletFX(position, target->position);
			target->takeDamage(garrison[member].type);
		}
		else {
			newBulletFX(position, (target->position-position).normalized(),garrison[member].type->range);
		}

		garrison[member].fireCooldown = (60.0 / garrison[member].type->fireRate);
		garrison[member].ammunition--;
		if (garrison[member].ammunition == 0) {
			garrison[member].fireCooldown = garrison[member].type->reloadTime;
			garrison[member].ammunition = garrison[member].type->magazineSize;
		}
	}
	else {
		targets[member] = nullptr;
	}
}

void building::findTargets(quadtree* tree) {

	std::vector<entity*> newTargets = findInRadius(tree, position, maxRange);

	for (int index = 0; index < garrison.size(); index++) {

		if (targets[index] == nullptr) {

			unit* closest = nullptr;
			double lowest = DBL_MAX;
			for (int i = 0; i < newTargets.size(); i++) {
				unit* u = dynamic_cast<unit*>(newTargets[i]);
				if (u != nullptr && u->team != team) {
					double range = (u->position - position).getMagnitude();
					if (range < garrison[index].type->range &&
						inFireArc(position, u->position, fieldsOfFire[index].direction, fieldsOfFire[index].arc)) {
						if (range < lowest) {
							closest = u;
							lowest = range;
						}
					}
				}
			}
			if (closest != nullptr) {
				targets[index] = closest;
			}
		}

	}
	
}

void building::addToGarrison(weapon* w, double arcAngle, double arc) {

	arcAngle = (arcAngle + ( (direction-3) *45) ) * M_PI / 180.0;
	arc *= M_PI / 180.0;
	maxRange = fmax(w->range, maxRange);

	garrison.push_back(WeaponData(w,w->magazineSize,0));
	fieldsOfFire.push_back(arcOfFire(vector2(cos(arcAngle), sin(arcAngle)), arc));
	targets.push_back(nullptr);
}
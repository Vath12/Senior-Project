#include "unit.h"
#include <algorithm>
#include <SDL.h>
#include "entity.h"
#include "sprite.h"
#include "vector2.h"
#include <iostream>
#include "render.h"
#include "util.h"
#include "god.h"
#include "effects.h"
#include "math.h"
#include "quadtree.h"
#include "combat.h"

unit::unit(sprite* _mainSprite, vector2 _position, int _direction) :
	entity(_mainSprite, _position, _direction) {

	velocity = vector2();
	destination = _position;
	dead = std::vector<sprite*>();
}

void unit::update(double deltaTime){

	vector2 d = (destination - position);

	if (attacking) {
		if (animation->frame == fireState.numFrames-1) {
			attacking = false;
			magazine--;
			if (magazine > 0) {
				attackCooldown = (60.0 / armament->fireRate);
			}
			else {
				magazine = armament->magazineSize;
				attackCooldown = armament->reloadTime * (1+ randomDouble()*0.2);
			}
		}
	}
	else if (moving) {
		mainSprite = moveAnimation;
		animation = &moveState;
		animation->animID = direction;
		if (d.getMagnitude() < 0.05) {
			moving = false;
			velocity = vector2();
			direction = destinationDirection;
		}
		else {
			d = d.normalized();
			velocity = velocity + (d * acceleration * deltaTime);
			velocity = velocity * (d * velocity.normalized());
			velocity = velocity.normalized() * std::clamp(velocity.getMagnitude(), 0.0, speed);
		}
		position = position + (velocity * deltaTime);

	}
	else {
		mainSprite = idleAnimation;
		animation = &idleState;
		animation->animID = direction;
	}

	if (attackReady() && target != nullptr) {
		vector2 d = target->position - position;
		if (d.getMagnitude() <= armament->range) {
			attack(target);
		}
		else {
			target = nullptr;
		}
	}

	entity::update(deltaTime);

	searchCooldown -= deltaTime;

	if (moving && armament != nullptr) {
		attackCooldown -= deltaTime*armament->movementSpeedPenalty;
	}
	else {
		attackCooldown -= deltaTime;
	}

	if (vp <= 0) {
		int z = (-1*dead.size())+1;
		for (sprite* s : dead){ 
			newCorpse(s, position, rand() % 8,z, s->frames,0.2+randomDouble()*0.2);
			z++;
		}
		destroy(this);
	}
}

void unit::findTarget(quadtree* tree) {

	std::vector<entity*> targets = findInRadius(tree, position, armament->range);
	int i = 0;
	int count = 5;
	double best = 10000000;
	unit* newTarget = nullptr;
	while (!targets.empty() && count > 0){

		(i = (abs(rand()) % targets.size()));

		unit* u = dynamic_cast<unit*>(targets[i]);
		if (u != nullptr && u->team != team) {
			double score = (u->position - position).getMagnitude();
			if (score < best-10) {
				newTarget = u;
				best = score;
				count--;
			}
		}
		targets[i] = targets.back();
		targets.pop_back();
	}
	if (newTarget != nullptr) {
		target = newTarget;
	}
	
}

bool unit::attackReady() {
	return armament != nullptr && !attacking && attackCooldown <= 0;
}

void unit::attack(unit* target) {

	vector2 d = target->position - position;
	direction = d.getCardinalDirection();

	bool hit = willHit(armament, position, target->position,target->velocity,moving);

	mainSprite = fireAnimation;
	animation = &fireState;
	animation->frame = 0;
	attacking = true;
	animation->animID = direction;

	if (hit) {
		newBulletFX(position, target->position);
		target->takeDamage(armament);
	}
	else {
		newBulletFX(position, d.normalized(), armament->range);
	}
}

void unit::takeDamage(weapon* source) {
	vp -= source->bluntDamage;
	if (source->penetration > ap) {
		vp -= source->postPenetrationDamage;
	}
}

void unit::moveTo(vector2 _destination) {
	destination = _destination;
	vector2 d = (destination - position);
	moving = true;
	direction = d.getCardinalDirection();
	velocity = vector2();
}

void unit::debugDraw(SDL_Renderer* renderer) {
	SDL_Rect R = worldToCamera(
		position.x,
		position.y,
		hitbox.x,
		hitbox.y );

	R.x = R.x + worldToCameraScale(hitboxCenter.x - hitbox.x/2.0);
	R.y = R.y + worldToCameraScale(hitboxCenter.y - hitbox.y/2.0);

	SDL_RenderDrawRect(renderer, &R);
}
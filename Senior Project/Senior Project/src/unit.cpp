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


unit::unit(sprite* _mainSprite, vector2 _position, int _direction) :
	entity(_mainSprite, _position, _direction) {
	velocity = vector2();
	destination = _position;
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
	

	entity::update(deltaTime);

	if (moving && armament != nullptr) {
		attackCooldown -= deltaTime*armament->movementSpeedPenalty;
	}
	else {
		attackCooldown -= deltaTime;
	}

	if (vp <= 0) {
		destroy(this);
	}
}

bool unit::attackReady() {
	return armament != nullptr && !attacking && attackCooldown <= 0;
}

bool willHit(weapon* w, double range, double lateralMovement,bool moving) {

	double rangeScalar = (range / w->range);
	double cappedLateralMovement = fmax(lateralMovement, 1);
	double rangeChance = ((1 - rangeScalar) * (1 - w->accuracy)) + w->accuracy;
	double closeChance = fmin(24*pow(rangeScalar+w->handling/cappedLateralMovement,2.0),1.0);
	double farChance = fmin((1 - (1 - w->leading) * pow(rangeScalar, 2.0)) * 1.0/(sqrt(fmax(lateralMovement, 1))),1.0);

	if (moving) {
		rangeChance *= w->movementAccuracyPenalty;
	}

	double chance = farChance * closeChance * rangeChance;
	return randomDouble() < chance;
}

void unit::attack(unit* target) {

	vector2 d = target->position - position;
	double range = d.getMagnitude();
	direction = d.getCardinalDirection();
	double lateral = (1 - fabs(d.normalized() * target->velocity.normalized())) * target->velocity.getMagnitude();

	bool hit = willHit(armament, range, lateral, moving);

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
		newBulletFX(position, position+ (d+ (randomVector() * 0.0001 * range)).normalized()*armament->range);
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
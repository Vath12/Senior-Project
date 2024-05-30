#include "unit.h"
#include <algorithm>
#include <SDL.h>
#include "entity.h"
#include "sprite.h"
#include "vector2.h"
#include <iostream>;
#include "render.h"
#include "util.h"

unit::unit(sprite* _mainSprite, vector2 _position, int _direction) :
	entity(_mainSprite, _position, _direction) {
	velocity = vector2();
	destination = _position;
}

void unit::update(double deltaTime){

	vector2 d = (destination - position);
	if (moving) {
		mainSprite = moveAnimation;
		animation = &moveState;
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
	}
	animation->animID = direction;

	entity::update(deltaTime);
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
		position.x + hitboxCenter.x - hitbox.x / 2.0,
		position.y + hitboxCenter.y - hitbox.y / 2.0,
		hitbox.x,
		hitbox.y );

	SDL_RenderDrawRect(renderer, &R);
}
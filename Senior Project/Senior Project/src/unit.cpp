#include "unit.h"
#include <algorithm>
#include <SDL.h>
#include "entity.h"
#include "sprite.h"
#include "vector2.h"
#include <iostream>;

unit::unit(sprite* _mainSprite, vector2 _position, int _direction) :
	entity(_mainSprite, _position, _direction) {
	velocity = vector2();
	destination = _position;
}

void unit::update(double deltaTime){

	if (moving) {
		mainSprite = moveAnimation;
		animation = &moveState;
		vector2 d = (destination - position);
		if (d.getMagnitude() < 0.1) {
			moving = false;
			velocity = vector2();
		}
		else {
			d = d.normalized();
			velocity = velocity + (d * acceleration * deltaTime);
			velocity = d * (d * velocity.normalized());
			velocity = velocity.normalized() * std::clamp(velocity.getMagnitude(), 0.0, speed);
		}
		position = position + (velocity * deltaTime);


	}
	else {
		mainSprite = idleAnimation;
		animation = &idleState;
	}

	entity::update(deltaTime);
}


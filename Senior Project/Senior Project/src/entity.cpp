#include <SDL.h>
#include <vector>
#include "entity.h"
#include <iostream>
#include "entity.h"
#include "vector2.h"

entity::entity(sprite* _mainSprite,vector2 _position, int _direction) {
	mainSprite = _mainSprite;
	position = _position;
	direction = _direction;
}

void entity::update(double deltaTime) {
	if (animating && animation != NULL) {
		if (animation->timer < 0) {
			animation->timer = 1.0 / animation->rate;
			animation->frame = (animation->frame + 1) % animation->numFrames;
		}
		animation->timer -= deltaTime;
	}
}

void entity::draw(SDL_Renderer* renderer) {
	if (animation != NULL) {
		mainSprite->x = position.x;
		mainSprite->y = position.y;
		mainSprite->draw(renderer, animation->frame, animation->animID);
	}
}

void entity::start() {

}

void entity::onDestroy() {

}


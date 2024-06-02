#include <SDL.h>
#include <vector>
#include "entity.h"
#include <iostream>
#include "entity.h"
#include "vector2.h"
#include "render.h"

entity::entity(sprite* _mainSprite,vector2 _position, int _direction) {
	mainSprite = _mainSprite;
	position = _position;
	direction = _direction;
	hitbox = vector2(1, 1);
	hitboxCenter = vector2(0, 0);
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
		mainSprite->draw(renderer, position, animation->frame, animation->animID,1);
	}
}

void entity::start() {

}

void entity::onDestroy() {

}


void entity::setFlag(unsigned long int id, bool state) {
	if (!state) {
		flags = flags & ~((unsigned long int)1 << id);
	}
	else {
		flags = flags | ((unsigned long int)1 << id);
	}
}
bool entity::getFlag(unsigned long int id) {
	return (flags >> id) & (unsigned long int) 1;
}

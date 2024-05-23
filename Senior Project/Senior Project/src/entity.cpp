#include <SDL.h>
#include <vector>
#include "entity.h"
#include <iostream>

std::vector<entity*> allEntities = std::vector<entity*>();

entity::entity() {

}

void entity::update(double deltaTime) {

}

void entity::draw(SDL_Renderer* renderer) {
	mainSprite->x = position.x;
	mainSprite->y = position.y;
	mainSprite->draw(renderer);
}

entity* entity::create(vector2 _position, int _direction, sprite* mainSprite) {
	entity* e = new entity();
	allEntities.push_back(e);
	e->position = _position;
	e->direction = _direction;
	e->mainSprite = mainSprite;
	return e;
}

void entity::destroy(entity* e) {
	for (int i = 0; i < allEntities.size(); i++) {
		if (e == allEntities[i]) {
			allEntities[i] = allEntities[allEntities.size() - 1];
			delete e;
			allEntities.pop_back();
			return;
		}
	}
	delete e;
}

void entity::freeAll() {
	int count = 0;
	while(allEntities.size() > 0) {
		destroy(allEntities[0]);
		count++;
	}
	std::cout << count << " entities deleted" << std::endl;
}

entity* entity::getEntity(int at) {
	return allEntities[at];
}
int entity::numEntities() {
	return allEntities.size();
}
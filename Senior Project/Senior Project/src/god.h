#pragma once
#include <vector>
class vector2;
class entity;
class sprite;
class group;

//template <typename T>
//T* create(sprite* _mainSprite, vector2 _position, int _direction);

extern std::vector<entity*> entities;
extern std::vector<group*> groups;

template <typename T>
T* create(sprite* _mainSprite, vector2 _position, int _direction) {
	static_assert(std::is_base_of<entity, T>::value, "type must be derived from entity");
	T* output = new T(_mainSprite, _position, _direction);
	entities.push_back(output);
	output->start();
	return output;
}


template <typename T>
void destroy(T* obj) {
	static_assert(std::is_base_of<entity, T>::value, "type must be derived from entity");
	for (int i = 0; i < entities.size(); i++) {
		if (obj == entities[i]) {
			entities[i] = entities.back();
			entities.pop_back();
			delete obj;
			return;
		}
	}
	delete obj;
}


group* createGroup();
void destroyGroup(group* obj);


void freeAll();

int numEntities();
entity* getEntity(int i);
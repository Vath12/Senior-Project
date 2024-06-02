#pragma once
#include <vector>
#include "unit.h"
#include "group.h"
#include <iostream>

class vector2;
class entity;
class sprite;


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

			/*
			unit* u = dynamic_cast<unit*>(obj);
			if (u != nullptr) {
				if (u->parent != nullptr) {
					u->parent->removeMember(u);
				}
			}
			*/
			

			std::cout << "deleting unit " << i << "/" << entities.size();

			entities[i] = entities.back();
			entities.pop_back();

			delete obj;

			std::cout << " : sucess" << std::endl;

			return;
		}
	}
	delete obj;
}


group* createGroup();
void destroyGroup(group* obj);


void freeAll();

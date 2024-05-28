#include "god.h"
#include <vector>
#include <iostream>
#include <type_traits>
#include "entity.h"
#include "unit.h"
#include "vector2.h"
#include "sprite.h"

std::vector<entity*> entities = std::vector<entity*>();


void freeAll() {
	int count = 0;
	while (!entities.empty()) {
		destroy(entities.back());
		count++;
	}
	std::cout << count << " entities deleted" << std::endl;
}

int numEntities() {
	return entities.size();
}
entity* getEntity(int i) {
	return entities[i];
}
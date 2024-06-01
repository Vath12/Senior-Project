#include "god.h"
#include <vector>
#include <iostream>
#include <type_traits>
#include "entity.h"
#include "unit.h"
#include "vector2.h"
#include "sprite.h"
#include "group.h"

std::vector<entity*> entities = std::vector<entity*>();
std::vector<group*> groups = std::vector<group*>();

group* createGroup() {
	group* output = new group();
	groups.push_back(output);
	return output;
}
void destroyGroup(group* obj) {
	for (int i = 0; i < groups.size(); i++) {
		if (obj == groups[i]) {
			groups[i] = groups.back();
			groups.pop_back();
			delete obj;
			return;
		}
	}
	delete obj;
}

void freeAll() {
	int count = 0;
	while (!entities.empty()) {
		destroy(entities.back());
		count++;
	}
	std::cout << count << " entities deleted" << std::endl;
	count = 0;
	while (!groups.empty()) {
		destroyGroup(groups.back());
		count++;
	}
	std::cout << count << " groups deleted" << std::endl;
}

#include "group.h"
#include <vector>
#include "unit.h"
#include "vector2.h"
#include "math.h";
#include "god.h"

std::vector<vector2> circleFormation(int units) {
	std::vector<vector2> output = std::vector<vector2>();
	double radius = 0;
	double angle = 0;
	double angleSpacing = 0;
	
	for (int i = 0; i < units; i++) {
		output.push_back(vector2(cos(angle)*radius,sin(angle)*radius));
		if (radius > 0) {
			angle += angleSpacing;
		}
		else {
			angle = 8;
		}

		if (angle > (M_PI * 2)-angleSpacing/2) {
			angle = 0.0;
			radius += 1.0;
			angleSpacing = (2 * M_PI) / (int)fmin((units-i),6*radius);
		}
	}
	
	return output;
}

std::vector<vector2> triangleFormation(int units) {
	return std::vector<vector2>();
}

group::group() {
	std::vector<unit*> members = std::vector<unit*>();
	position = vector2();
}


void group::update() {
	if (!members.empty()) {
		position = vector2(0.0,0.0);
		dispersion = 0.0;
		for (unit* u : members) {
			position = position + u->position;
		}
		position = position / members.size();
		for (unit* u : members) {
			dispersion = fmax(dispersion, (u->position-position).getMagnitude());
		}
	}
}



void group::setFlag(unsigned long int id,bool state) {
	for (unit* u : members) {
		u->setFlag(id, state);
	}
}

void group::removeMember(unit* oldMember) {
	for (int i = 0; i < members.size(); i++) {
		if (members[i] == oldMember) {
			members[i] = members.back();
			members.pop_back();
			break;
		}
	}
	if (members.empty()) {
		destroyGroup(this);
		return;
	} 
}

void group::addMember(unit* newMember) {
	if (members.empty()) {
		team = newMember->team;
	}
	if (newMember->armament != nullptr) {
		maxRange = fmax(maxRange, newMember->armament->range);
	}
	members.push_back(newMember);
	newMember->parent = this;
}
void group::moveTo(vector2 destination) {
	int i = 0;
	for (vector2 pos : circleFormation(members.size())) {
		members[i]->relativePosition = pos;
		i++;
	}
	for (unit* u: members) {
		u->moveTo(destination + u->relativePosition);
	}
}
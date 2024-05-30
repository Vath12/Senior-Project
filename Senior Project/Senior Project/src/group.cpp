#include "group.h"
#include <vector>
#include "unit.h"
#include "vector2.h"
#include "math.h";

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
}

void group::setFlag(unsigned long int id,bool state) {
	for (unit* u : members) {
		u->setFlag(id,state);
	}
}

void group::addMember(unit* newMember) {
	members.push_back(newMember);
	newMember->parent = this;

	int i = 0;
	for (vector2 pos : circleFormation(members.size())) {
		members[i]->relativePosition = pos;
		i++;
	}

}
void group::moveTo(vector2 destination) {
	for (unit* u: members) {
		u->moveTo(destination+u->relativePosition);
	}
}
#pragma once
#include <vector>
#include "vector2.h"

class unit;
class group
{

	public:
		vector2 position;
		double dispersion = 0.0;
		std::vector<unit*> members;
		int team = 0;
		double maxRange = 0;
		bool selected = false;
		group();
		void setFlag(unsigned long int id, bool state);
		void addMember(unit* newMember);
		void removeMember(unit* oldMember);
		void moveTo(vector2 destination);
		void update();

};

std::vector<vector2> circleFormation(int units);

std::vector<vector2> triangleFormation(int units);


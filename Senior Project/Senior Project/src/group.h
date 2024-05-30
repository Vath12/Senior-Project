#pragma once
#include <vector>

class unit;
class vector2;

class group
{

	public:
		std::vector<unit*> members;
		bool selected = false;
		group();
		void setFlag(unsigned long int id, bool state);
		void addMember(unit* newMember);
		void moveTo(vector2 destination);

};

std::vector<vector2> circleFormation(int units);

std::vector<vector2> triangleFormation(int units);


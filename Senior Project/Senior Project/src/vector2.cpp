#include "vector2.h"
#include "globals.h"
#include <math.h>

vector2::vector2() {
	x = 0.0;
	y = 0.0;
}
vector2::vector2(double _x, double _y){
	x = _x;
	y = _y;
}

vector2 vector2::operator*(double b) {
	return vector2(x*b,y*b);
}

vector2 vector2::operator/(double b) {
	return vector2(x/b, y/b);
}

double vector2::getMagnitude() {
	return sqrt(pow(x,2)+pow(y,2));
}

vector2 vector2::normalized() {
	double mag = getMagnitude();
	if (mag == 0) {
		return vector2(0, 0);
	}
	return vector2(x,y)/mag;
}

vector2 vector2::operator+(vector2 b) {
	return vector2(x+b.x,y+b.y);
}

vector2 vector2::operator-(vector2 b) {
	return vector2(x - b.x, y - b.y);
}

vector2 vector2::operator*(vector2 b) {
	return vector2(x * b.x, y * b.y);
}

bool vector2::operator!=(vector2 b) {
	return (b.x != x) || (b.y != y);
}
bool vector2::operator==(vector2 b) {
	return (b.x == x) && (b.y == y);
}

int vector2::getCardinalDirection() {

	if (x == 0) {
		if (y >= 0) {
			return compass::S;
		}
		else {
			return compass::N;
		}
	}
	double slope = (y/x);
	if (slope < -2.414) {
		if (y>0) {
			return compass::N;
		}
		return compass::S;
	}
	if (slope < -.414) {
		if (y > 0) {
			return compass::NE;
		}
		return compass::SE;
	}
	if (slope < .414) { 
		if (y > 0) {
			return compass::E;
		}
		return compass::E;
	}
	if (slope < 2.414) {
		return compass::N;
	}
}
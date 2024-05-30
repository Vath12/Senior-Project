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

double vector2::operator*(vector2 b) {
	return x * b.x + y * b.y;
}

bool vector2::operator!=(vector2 b) {
	return (b.x != x) || (b.y != y);
}
bool vector2::operator==(vector2 b) {
	return (b.x == x) && (b.y == y);
}

int vector2::getCardinalDirection() {

	double angle = atan2(y,x)+3.141592654;

	return fmod(round(angle*(4/3.141592654)+6),8.0);

}
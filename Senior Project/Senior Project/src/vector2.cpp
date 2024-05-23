#include "vector2.h"
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
	return vector2(x,y)/getMagnitude();
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


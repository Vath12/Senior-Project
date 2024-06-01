#include "util.h"
#include "math.h"
#include "vector2.h"
#include <random>

bool pointInBox(vector2 point, vector2 boxCenter, vector2 extents) {
	return (fabs(point.x - boxCenter.x) <= extents.x) && (fabs(point.y - boxCenter.y) <= extents.y);
}


double randomDouble() {
	return (rand() % 1000) / 1000.0;
}
vector2 randomVector() {
	return vector2(randomDouble(), randomDouble());
}
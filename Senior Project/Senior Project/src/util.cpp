#include "util.h"
#include "math.h"
#include "vector2.h"

bool pointInBox(vector2 point, vector2 boxCenter, vector2 extents) {
	return (fabs(point.x - boxCenter.x) < extents.x) && (fabs(point.y - boxCenter.y) < extents.y);
}
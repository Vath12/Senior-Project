#include "combat.h"
#include "math.h"
#include "util.h"
#include "vector2.h"
#include <iostream>

bool willHit(weapon* w, vector2 muzzle, vector2 target, vector2 relativeVelocity, bool moving) {
	vector2 d = (target - muzzle);
	double range = d.getMagnitude();
	double lateral = (1 - fabs(d.normalized() * relativeVelocity.normalized())) * relativeVelocity.getMagnitude();
	return willHit(w,range,lateral,moving);
}


bool willHit(weapon* w, double range, double lateralMovement, bool moving) {

	double rangeScalar = (range / w->range);
	double cappedLateralMovement = fmax(lateralMovement, 1);
	double rangeChance = pow(0.5,rangeScalar/w->accuracy);
	double closeChance = fmin(24 * pow(rangeScalar + w->handling / cappedLateralMovement, 2.0), 1.0);
	double farChance = fmin((1 - (1 - w->leading) * pow(rangeScalar, 2.0)) * 1.0 / (sqrt(fmax(lateralMovement, 1))), 1.0);

	if (moving) {
		rangeChance *= w->movementAccuracyPenalty;
	}
	double chance = farChance * closeChance * rangeChance;
	return randomDouble() < chance;
}

bool inFireArc(vector2 position, vector2 target, vector2 arcCenter,double arc) {
	vector2 d = (target - position).normalized();
	double dot = arcCenter * d;
	return acos(dot) < arc;
}


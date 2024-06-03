#pragma once
#include "vector2.h"

struct weapon {
	double bluntDamage = 0;
	double penetration = 0;
	double postPenetrationDamage = 0;
	double range;
	//hit targets at max range
	double accuracy;
	//hit moving targets at close range
	double handling;
	//hit moving targets at long range
	double leading;
	double fireRate;
	int magazineSize;
	double reloadTime;
	double movementSpeedPenalty;
	double movementAccuracyPenalty;
};

struct arcOfFire {
	vector2 direction;
	double arc;
};

struct WeaponData {
	weapon* type;
	int ammunition;
	double fireCooldown;
};

bool inFireArc(vector2 position, vector2 target, vector2 arcCenter, double arc);
bool willHit(weapon* w, double range, double lateralMovement, bool moving);
bool willHit(weapon* w, vector2 muzzle, vector2 target, vector2 relativeVelocity, bool moving);


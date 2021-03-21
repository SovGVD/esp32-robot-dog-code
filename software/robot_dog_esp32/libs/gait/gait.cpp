/**
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */

#include "gait.h"

gait::gait(gaitConfig &config, leg &legObj)
{
	_config = &config;
	_leg    = &legObj;
}

/**
 * Return state if this gait is in progress
 */
double gait::next() {
	if (ticksToStop > 0) {
		ticksToStop--;
		progress = 1 - (float)ticksToStop/(float)ticksMax;
		
		//_transition.swing(progress);
		_leg->foot = _transition.swing(progress);
		_leg->sensor.onGround = false;
	} else {
		progress = 0;
		// TODO use real sesors and stop gait if leg touch the ground
		_leg->sensor.onGround = true;
	}
	
	return progress;
}

void gait::start(point from, point to) {
	ticksToStop = _config->swingDuration/_config->loopTime;
	ticksMax    = ticksToStop;
	
	tParams = {
		from,
		to,
		_config->offTheGround
	};
	// TODO add somethiong to compare point
	// TODO YAW did npot work properly with that
	if (from.x == to.x && from.y == to.y && from.z == to.z) {
		// don't move if from and to equal
		ticksToStop = 0;
	}
	_transition.set(tParams);
}

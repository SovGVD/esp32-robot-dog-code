/**
 * Transitions for leg movement
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */

#include "math.h"
#include "transition.h"

transition::transition()
{
}

void transition::set(transitionParameters param)
{
	_param = param;
	ax = _param.targetValue.x - _param.initialValue.x;
	ay = _param.targetValue.y - _param.initialValue.y;
	az = _param.targetValue.z - _param.initialValue.z;
	
	z1 = _param.initialValue.z + _param.offTheGround;
	z2 = _param.targetValue.z  + _param.offTheGround;
}


/**
 * progress [0,1];
 */
point transition::swing(double progress)
{
	p.x = progress*ax + _param.initialValue.x;
	p.y = progress*ay + _param.initialValue.y;
	
	if (progress <= TRANSITION_PROGRESS_STEP1) {
		stepProgress = progress/TRANSITION_PROGRESS_STEP1;
		az = z1 - _param.initialValue.z;
		z  = _param.initialValue.z;
		
	} else if (progress <= TRANSITION_PROGRESS_STEP2) {
		stepProgress = (progress - TRANSITION_PROGRESS_STEP1)/(TRANSITION_PROGRESS_STEP2 - TRANSITION_PROGRESS_STEP1);
		az = z2 - z1;
		z  = z1;
		
	} else {
		stepProgress = (progress - TRANSITION_PROGRESS_STEP2)/(1 - TRANSITION_PROGRESS_STEP2);
		az = _param.targetValue.z - z2;
		z  = z2;
		
	}
	p.z = stepProgress*az + z;
	
	//Serial.print(progress*100);
	//Serial.print(" ");
	//Serial.print(stepProgress*100);
	//Serial.print(" ");
	//Serial.println(p.z);
	
	return p;
}

point transition::linear(double progress)
{
	p.x = progress*ax + _param.initialValue.x;
	p.y = progress*ay + _param.initialValue.y;
	p.z = progress*az + _param.initialValue.z;

	return p;
}

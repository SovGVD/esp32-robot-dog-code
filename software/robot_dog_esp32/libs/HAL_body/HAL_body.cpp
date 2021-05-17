#include "math.h" 
#include "HAL_body.h"

HAL_body::HAL_body(moveVector &vector, figure &bodyObj, leg legs[LEG_NUM])
{
	_vector = &vector;	// TODO that is wrong... we should set body pitch/roll somewhere else (e.g. in IMU process that still not in the code...)
	_body   = &bodyObj;
	for (int i = 0; i < LEG_NUM; i++) {
		_legs[i] = &legs[i];
	}
}

void HAL_body::update()
{
	/**
	 * x' = x * (cosYaw*cosPitch) + y * (cosYaw*sinPitch*sinRoll - sinYaw*cosRoll) + z * (cosYaw*sinPitch*cosRoll + sinYaw*sinRoll)
	 * y' = x * (sinYaw*cosPitch) + y * (sinYaw*sinPitch*sinRoll + cosYaw*cosRoll) + z * (sinYaw*sinPitch*cosRoll - cosYaw*sinRoll)
	 * z' = x * (-sinPitch)       + y * (cosPitch*sinRoll)                         + z * (cosPitch*cosRoll)
	 * @see: https://en.wikipedia.org/wiki/Rotation_matrix#General_rotations
	 */
	
	double sinPitch = sin(_body->orientation.pitch + _vector->rotate.pitch * 0.2);	// TODO THIS IS WRONG, it should not be there!!!
	double cosPitch = cos(_body->orientation.pitch + _vector->rotate.pitch * 0.2);

	double sinRoll = sin(_body->orientation.roll + _vector->rotate.roll * 0.2);
	double cosRoll = cos(_body->orientation.roll + _vector->rotate.roll * 0.2);

	double sinYaw = sin(_body->orientation.yaw);
	double cosYaw = cos(_body->orientation.yaw);

	for (int i = 0; i < LEG_NUM; i++) {		
		_legs[i]->body.x = _body->position.x 
						 + _legs[i]->defaultBody.x * (cosYaw*cosPitch)
						 + _legs[i]->defaultBody.y * (cosYaw*sinPitch*sinRoll - sinYaw*cosRoll)
						 + _legs[i]->defaultBody.z * (cosYaw*sinPitch*cosRoll + sinYaw*sinRoll);

		_legs[i]->body.y = _body->position.y
						 + _legs[i]->defaultBody.x * (sinYaw*cosPitch)
						 + _legs[i]->defaultBody.y * (sinYaw*sinPitch*sinRoll + cosYaw*cosRoll)
						 + _legs[i]->defaultBody.z * (sinYaw*sinPitch*cosRoll - cosYaw*sinRoll);

		_legs[i]->body.z = _body->position.z
						 + _legs[i]->defaultBody.x * (-sinPitch)
						 + _legs[i]->defaultBody.y * (cosPitch*sinRoll)
						 + _legs[i]->defaultBody.z * (cosPitch*cosRoll);
	}

}

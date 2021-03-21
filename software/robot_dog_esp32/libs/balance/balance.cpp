/**
 * Balance helper methods
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */

#include "balance.h"

balance::balance(point &offset, figure &bodyObj, leg &legLF, leg &legRF, leg &legLH, leg &legRH)
{
	_offset = &offset;	// this should be calculated? base on servos and legs positions and weight
	_body   = &bodyObj;
	_legLF  = &legLF;
	_legRF  = &legRF;
	_legLH  = &legLH;
	_legRH  = &legRH;
}


point balance::getCenter()
{
	// TODO... can we ignore Z? as it will be projection of BODY center of mass (CoM) to legs-on-ground plain
	// This is not technicaly correct, we are not geting center of mass, but just the point where it should be as projection
	_legsOnGround = 0;
	_CoM = {0, 0, 0};
	
	// this is terrible
	_legToBalance(_legLF);
	_legToBalance(_legRF);
	_legToBalance(_legLH);
	_legToBalance(_legRH);
	


	_CoM.x = _CoM.x/_legsOnGround + _offset->x;
	_CoM.y = _CoM.y/_legsOnGround + _offset->y;
	_CoM.z = _CoM.z/_legsOnGround + _offset->z;
	
	/*Serial.print(_legsOnGround);	
	Serial.print("|CoM ");
	Serial.print(_CoM.x);
	Serial.print(",");
	Serial.print(_CoM.y);
	Serial.print(",");
	Serial.println(_CoM.z);*/
	
	return _CoM;	
}

void balance::_legToBalance(leg *_leg) {
	if (_leg->sensor.onGround) {
		_CoM.x += _leg->foot.x;
		_CoM.y += _leg->foot.y;
		_CoM.z += _leg->foot.z;
		_legsOnGround++;
	}	
}

void balance::setBody(point CoM)
{
	// TODO that is kind of wrong, as we need to use body projection
	_body->position.x = CoM.x;
	_body->position.y = CoM.y;
	//_body->position.z = CoM.z;
}

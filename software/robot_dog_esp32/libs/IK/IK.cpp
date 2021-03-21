/**
 * Inverse Kinematics for quadruped (four legs) robot
 * Boston Dynamics Spot style
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */

#include "math.h" 
#include "leg.h"
#include "IK.h"

IK::IK(leg &legObj, figure &bodyObj)
{
	_leg  = &legObj;
	_body = &bodyObj;
}

void IK::set(leg &legObj, figure &bodyObj)
{
	_leg  = &legObj;
	_body = &bodyObj;
}

iksolver IK::solve()
{
	legangle angle;
	iksolver s;
	
	// TODO: what can I do with limits?
	double lx = _leg->foot.x - (_leg->body.x + _body->position.x + bodyBalance.x); if (_leg->inverse.x) { lx = -lx; };
	double ly = _leg->foot.y - (_leg->body.y + _body->position.y + bodyBalance.y); if (_leg->inverse.y) { ly = -ly; };
	double lz = _leg->foot.z - (_leg->body.z + _body->position.z + bodyBalance.z); if (_leg->inverse.z) { lz = -lz; };
	#ifdef DEBUG_HAL_LEG
		Serial.print("LEG BODY: ");
		Serial.print(_leg->body.x, 10);
		Serial.print(" ");
		Serial.print(_leg->body.y, 10);
		Serial.print(" ");
		Serial.println(_leg->body.z, 10);
		
		Serial.print("LEG POSITION: ");
		Serial.print(_body->position.x, 10);
		Serial.print(" ");
		Serial.print(_body->position.y, 10);
		Serial.print(" ");
		Serial.println(_body->position.z, 10);
		
		
		Serial.print("LEG FOOT: ");
		Serial.print(_leg->foot.x, 10);
		Serial.print(" ");
		Serial.print(_leg->foot.y, 10);
		Serial.print(" ");
		Serial.println(_leg->foot.z, 10);
		
		Serial.print("L: ");
		Serial.print(lx, 10);
		Serial.print(" ");
		Serial.print(ly, 10);
		Serial.print(" ");
		Serial.println(lz, 10);
	#endif

	
	double a = lx*lx + lz*lz;                       // square of hypotenuse (points between leg.body and leg.foot in XZ-plane)
	double sqrta = sqrt(a);
	double dxz = a - _leg->size.l1*_leg->size.l1;   // square of hypotenuse between BETA angle joint and ground in XZ plane
	double dyz = ly*ly + dxz;                       // square of hypotenuse between BETA angle joint and ground in YZ plane
	double l2p2 = _leg->size.l2*_leg->size.l2;      // square of l2
	double l3p2 = _leg->size.l3*_leg->size.l3;      // square of l3
	#ifdef DEBUG_HAL_LEG	
		Serial.print("A: ");
		Serial.print(a, 10);
		Serial.print(" dXZ: ");
		Serial.print(dxz, 10);
		Serial.print(" dYZ: ");
		Serial.println(dyz, 10);
	#endif
	
	angle.alpha = M_PI - ikAsin(lx/sqrta) - ikAcos(_leg->size.l1/sqrta);
	#ifdef DEBUG_HAL_LEG
		Serial.print("ALPHA: ");
		Serial.println(angle.alpha*(180/M_PI), 10);
		Serial.print("ALPHA DBG:");
		Serial.print(ikAtan2(lz, lx),10);
		Serial.print(" ");
		Serial.println(ikAcos(_leg->size.l1/sqrt(a)),10);
	#endif
	
	angle.beta  = M_PI_2 - ikAcos( (l3p2 - l2p2 - dyz) / (-2 * sqrt(dyz) * _leg->size.l2)) - ikAsin(ly/sqrt(dxz)); 
	#ifdef DEBUG_HAL_LEG
		Serial.print("BETA: ");
		Serial.println(angle.beta*(180/M_PI), 10);
	#endif

	angle.gamma = ikAcos( (dyz - l2p2 - l3p2) / (-2 * _leg->size.l2 * _leg->size.l3) );
	#ifdef DEBUG_HAL_LEG
		Serial.print("GAMMA: ");
		Serial.println(angle.gamma*(180/M_PI), 10);
	#endif
	
	s.isSolved = true;	// TODO
	s.angle = angle;
	
	return s;
}

/*point solveByAngle()
{
	// TODO
}*/

double IK::normalizeAngleRad(double angleRad)
{
	// TODO It is not working as expected
    //angleRad = fmod(angleRad,M_2_PI);
    //if (angleRad < 0) angleRad += M_2_PI;
    return angleRad;
}

// this can be update to use table of angles (faster) insted of using trigonometry functions
// or at least add cache
double IK::ikAtan2(double x, double y)
{
	return normalizeAngleRad(atan2(x, y));
}

double IK::ikAcos(double angleRad)
{
	return normalizeAngleRad(acos(angleRad));
}

double IK::ikAsin(double angleRad)
{
	return normalizeAngleRad(asin(angleRad));
}

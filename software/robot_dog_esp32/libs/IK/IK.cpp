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
	
	// normalize by body rotation
	double tmpSin = sin(_body->orientation.yaw * -1);
	double tmpCos = cos(_body->orientation.yaw * -1);

	point legFoot = {
			(_leg->foot.x - _body->position.x) * tmpCos - (_leg->foot.y - _body->position.y) * tmpSin,
			(_leg->foot.x - _body->position.x) * tmpSin + (_leg->foot.y - _body->position.y) * tmpCos,
			_leg->foot.z
		};

	point legBody = {
			(_leg->body.x - _body->position.x) * tmpCos - (_leg->body.y - _body->position.y) * tmpSin,
			(_leg->body.x - _body->position.x) * tmpSin + (_leg->body.y - _body->position.y) * tmpCos,
			_leg->body.z
		};


	// TODO: what can I do with limits?
	double lx = legFoot.x - legBody.x; if (_leg->inverse.x) { lx = -lx; };
	double ly = legFoot.y - legBody.y; if (_leg->inverse.y) { ly = -ly; };
	double lz = legFoot.z - legBody.z; if (_leg->inverse.z) { lz = -lz; };

	double a = lx*lx + lz*lz;                       // square of hypotenuse (points between leg.body and leg.foot in XZ-plane)
	double sqrta = sqrt(a);
	double dxz = a - _leg->size.l1*_leg->size.l1;   // square of hypotenuse between BETA angle joint and ground in XZ plane
	double dyz = ly*ly + dxz;                       // square of hypotenuse between BETA angle joint and ground in YZ plane
	double l2p2 = _leg->size.l2*_leg->size.l2;      // square of l2
	double l3p2 = _leg->size.l3*_leg->size.l3;      // square of l3
	
	angle.alpha = M_PI - ikAsin(lx/sqrta) - ikAcos(_leg->size.l1/sqrta);
	
	angle.beta  = M_PI_2 - ikAcos( (l3p2 - l2p2 - dyz) / (-2 * sqrt(dyz) * _leg->size.l2)) - ikAsin(ly/sqrt(dxz)); 

	angle.gamma = ikAcos( (dyz - l2p2 - l3p2) / (-2 * _leg->size.l2 * _leg->size.l3) );
	
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

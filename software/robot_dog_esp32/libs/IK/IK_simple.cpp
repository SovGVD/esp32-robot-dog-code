/**
 * Inverse Kinematics for quadruped (four legs) robot
 * Boston Dynamics Spot style
 * 
 * This IK is simple because leg is in one plane.
 * 
 *                _
 *              .|\      .
 *     -----| .    \      . L           -----|
 * --- -----0/ \    a      . E          -----0--. ---
 *  ^  ----/..  \    \      . G         ----/.  |  ^
 *  |  ---` . .  \   _\|     .          ---` .  |  |
 *  lz      .  . /   .        . P            0--`  a
 *  |       .   0   .          . L           |     |     ^Z
 *  v       .    \ .            . A          |     v     |
 * -------- .     0              . N         0 ------    0-->X
 *          |<lx->|               . E
 *                                 .
 * 
 * LEG PLANE
 * 
 *  -----0             0
 *  ^     `.           ^`.
 *  |       `.         \  `.
 *  a         0         b   0
 *  |        /           \ /       ^legplane
 *  v       /            v/        |
 *  -------0             0     Y<--0
 *  |<-ly->|
 * 
 * 
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2021
 */

#include "math.h"
#include "leg.h"
#include "IK_simple.h"

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
	// We need it to make IK simple ignoring body transitions, and at the same time still have absolute positions for each robot points
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


	double a = sq(lx) + sq(lz);        // square of hypotenuse (points between leg.body and leg.foot in XZ-plane)
	double sqrta = sqrt(a);
	double b = sq(ly) + a;             // square of hypotenuse (points between leg.body and leg.foot in YLeg-plane)
	double sqrtb = sqrt(b);            // square of hypotenuse (points between leg.body and leg.foot in YZ-plane)
	double l2p2 = sq(_leg->size.l2);   // square of l2		TODO this is const actually
	double l3p2 = sq(_leg->size.l3);   // square of l3		TODO this is const actually


	// XZ plane
	angle.alpha = M_PI_2 - ikAsin(lx/sqrta);

	// YLeg plane
	angle.beta  = M_PI_2 - ikAsin(ly/sqrtb) - ikAcos( (b+l2p2-l3p2) / (2 * sqrtb * _leg->size.l2));

	// YLeg plane
	angle.gamma = ikAcos((l2p2 + l3p2 - b) / (2 * _leg->size.l2 * _leg->size.l3));

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

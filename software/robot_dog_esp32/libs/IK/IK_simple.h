/**
 * Inverse Kinematics for quadruped (four legs) robot
 * Boston Dynamics Spot style
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */
 
 // WARNING: this is WIP and this library was not tested in any software or hardware!!!
 
 /**
 * All axis aligned with MPU9250
 *
 *
 *               |   Z-> Y
 *               |   |
 *    --------0-+   v X
 *            | ...l1
 *        0---0
 *
 *
 *      beta.                 alpha.
 *           .                      .
 *            .  |   ^ Z             .|         ^ Z
 *             0 |   |          ^  0--0         |
 *     ------ / -+   X-> Y      .  |  +---  X <-Y
 *           / ...l2            d  |
 * gamma...0                    .  0
 *          \                   .  |
 *           \ ...l3            v  |
 *
 *
 */
 
/**
 *          0---0 (legLH)       0---0 (legLF)
 *              |               |
 *           +--0---------------0--+
 *           |                     |
 *           |                     |
 *           |                     |
 *           +--0---------------0--*
 *              |               |
 *          0---0 (legRH)       0---0 (legRF)
 */
#ifndef IK_h
#define IK_h

#include "geometry.h"
#include "leg.h"

typedef struct {
	bool isSolved;
	legangle angle;
} iksolver;

class IK
{
	public:
		IK(leg &legObj, figure &bodyObj);
		void set(leg &legObj, figure &bodyObj);
		iksolver solve();
		//point solveByAngle();	// use to init position by initial angles
		double normalizeAngleRad(double angleRad);
	private:
		leg *_leg;
		figure *_body;
		double ikAtan2(double x, double y);
		double ikAcos(double angleRad);
		double ikAsin(double angleRad);
		point bodyBalance = {0, 0, 0};	// TODO this is just for tests
};

#endif

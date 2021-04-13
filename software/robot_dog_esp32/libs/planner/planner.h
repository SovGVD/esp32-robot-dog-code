/**
 * Planner
 * Almost, it is just move body and legs to the new point
 * Also it looks like a good place to check if new possition possible
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */

#ifndef planner_h
#define planner_h

#include "../IK/geometry.h"
#include "../IK/leg.h"

class planner
{
	public:
		planner(moveVector &vector, figure &bodyObj, leg &legLF, leg &legRF, leg &legLH, leg &legRH);
		void predictPosition(uint8_t steps);
		figure getBodyPosition();
		point getLegPosition(uint8_t legId);
	private:
		moveVector *_vector;
		figure     *_body;
		leg        *_legLF;
		leg        *_legRF;
		leg        *_legLH;
		leg        *_legRH;
		figure     _predictedBody;
		point      _predictedLegLFfoot;
		point      _predictedLegRFfoot;
		point      _predictedLegLHfoot;
		point      _predictedLegRHfoot;
		double     moveInc = 30;
		double     rotateInc = -0.3;
};

#endif

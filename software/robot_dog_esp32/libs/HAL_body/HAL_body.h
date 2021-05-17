/**
 * Body state recalculate
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2021
 */
 
#ifndef HAL_BODY_h
#define HAL_BODY_h

#include "../IK/geometry.h"
#include "../IK/leg.h"

class HAL_body
{
	public:
		HAL_body(moveVector &vector, figure &bodyObj, leg legs[LEG_NUM]);
		void update();
	private:
		moveVector *_vector;
		figure     *_body;
		leg        *_legs[LEG_NUM];

};

#endif

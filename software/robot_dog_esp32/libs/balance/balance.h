/**
 * Balance helper methods
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */

#ifndef balance_h
#define balance_h

#include "../IK/geometry.h"
#include "../IK/leg.h"

class balance
{
	public:
		balance(point &offset, figure &bodyObj, leg &legLF, leg &legRF, leg &legLH, leg &legRH);
		point getCenter();
		void setBody(point CoM);
	private:
		point   *_offset;
		figure  *_body;
		leg     *_legLF;
		leg     *_legRF;
		leg     *_legLH;
		leg     *_legRH;
		uint8_t _legsOnGround;
		point   _CoM;
		void _legToBalance(leg *_leg);
};

#endif

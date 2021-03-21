double displayHALMovement = 1;  // mm
double displayHALRotation = M_PI/180;

// tests
double HALTEST1_dx    = 0;
double HALTEST1_dy    = 0;
double HALTEST1_dz    = 0;
double HALTEST1_d     = 1;
int HALTEST1_stage = 0;
double HALTEST1_delta = 49.9; // +-50 mm (square 100x100)

void testHAL(int test) {
  switch (HALTEST1_stage) {
    case 0:
      HALTEST1_dy = HALTEST1_dy + HALTEST1_d;
      if (HALTEST1_dy >= HALTEST1_delta) HALTEST1_stage++;
      break;
    case 1:
      HALTEST1_dx = HALTEST1_dx + HALTEST1_d;
      if (HALTEST1_dx >= HALTEST1_delta) HALTEST1_stage++;
      break;
    case 2:
      HALTEST1_dy = HALTEST1_dy - HALTEST1_d;
      if (HALTEST1_dy <= -HALTEST1_delta) HALTEST1_stage++;
      break;
    case 3:
      HALTEST1_dx = HALTEST1_dx - HALTEST1_d;
      if (HALTEST1_dx <= -HALTEST1_delta) HALTEST1_stage=0;
      break;
  }

  testHAL_LEGLF(legs[LEGLF]);
  //testHAL_LEGRF(legs[LEGRF]);
  //testHAL_LEGLH(legs[LEGLH]);
  //testHAL_LEGRH(legs[LEGRH]);
}

void testHAL_LEGLF(leg &_leg) {
  _leg.foot.x = -LEG_POINT_X   + HALTEST1_dx;
  _leg.foot.y =  LEG_POINT_Y_F + HALTEST1_dy;
  _leg.foot.z = -LEG_POINT_Z   + HALTEST1_dz;
}

void testHAL_LEGRF(leg &_leg) {
  _leg.foot.x =  LEG_POINT_X   + HALTEST1_dx;
  _leg.foot.y =  LEG_POINT_Y_F + HALTEST1_dy;
  _leg.foot.z = -LEG_POINT_Z   + HALTEST1_dz;
}


void testHAL_LEGLH(leg &_leg) {
  _leg.foot.x = -LEG_POINT_X   + HALTEST1_dx;
  _leg.foot.y = -LEG_POINT_Y_H + HALTEST1_dy;
  _leg.foot.z = -LEG_POINT_Z   + HALTEST1_dz;
}

void testHAL_LEGRH(leg &_leg) {
  _leg.foot.x =  LEG_POINT_X   + HALTEST1_dx;
  _leg.foot.y = -LEG_POINT_Y_H + HALTEST1_dy;
  _leg.foot.z = -LEG_POINT_Z   + HALTEST1_dz;
}

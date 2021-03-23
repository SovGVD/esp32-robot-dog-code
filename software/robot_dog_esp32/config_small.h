
/**
 * Specific for robot config
 */
// default legs state (TODO measure it!!!)
#define LEG_BODY_X      39.5
#define LEG_BODY_Y_F    70.5  // not sure
#define LEG_BODY_Y_H    95.0
#define LEG_BODY_Z       0.0
#define LEG_POINT_X     39.5
#define LEG_POINT_Y_F   70.5 // not sure
#define LEG_POINT_Y_H   95.0
#define LEG_POINT_Z     75.0

point balanceOffset = {0, 0, 0};

leg legs[LEG_NUM] = {
  // LEFT FRONT
  {
    {LEGLF, "LF"},
    { -LEG_BODY_X,  LEG_BODY_Y_F,   LEG_BODY_Z},
    {-LEG_POINT_X, LEG_POINT_Y_F, -LEG_POINT_Z},
    { LEG_SIZE_L1,   LEG_SIZE_L2,  LEG_SIZE_L3},
    {LEG_ANGLE_ALPHA_MIN, LEG_ANGLE_BETA_MIN, LEG_ANGLE_GAMMA_MIN},
    {LEG_ANGLE_ALPHA_MAX, LEG_ANGLE_BETA_MAX, LEG_ANGLE_GAMMA_MAX},
    {
      {M_PI_2, M_PI_4, M_PI_2},  // hardware middle
      { 25, 26, 27},             // 3 servo pins
      {  0,  0,  0},             // servo middle trim
      {  2,  1,  1}              // gear ratio, first and last one is 16 groves on servo and 24 groves on leg
    },
    {   -LEG_POINT_X,  LEG_POINT_Y_F,    -LEG_POINT_Z},
    {LEG_ANGLE_ALPHA, LEG_ANGLE_BETA, LEG_ANGLE_GAMMA},
    {true, true, false, false, false, true},
    {true, 0, 0}
  },
  // RIGHT FRONT
  {
    {LEGRF, "RF"},
    { LEG_BODY_X,  LEG_BODY_Y_F,   LEG_BODY_Z},
    {LEG_POINT_X, LEG_POINT_Y_F, -LEG_POINT_Z},
    {LEG_SIZE_L1,   LEG_SIZE_L2,  LEG_SIZE_L3},
    {LEG_ANGLE_ALPHA_MIN, LEG_ANGLE_BETA_MIN, LEG_ANGLE_GAMMA_MIN},
    {LEG_ANGLE_ALPHA_MAX, LEG_ANGLE_BETA_MAX, LEG_ANGLE_GAMMA_MAX},
    {
      {M_PI_2, M_PI_4, M_PI_2},
      { 16, 18, 17},
      {  0,  0,  0},
      {  2,  1,  1}
    },
    {    LEG_POINT_X,  LEG_POINT_Y_F,    -LEG_POINT_Z},
    {LEG_ANGLE_ALPHA, LEG_ANGLE_BETA, LEG_ANGLE_GAMMA},
    {false, true, false, true, true, false},
    {true, 0, 0}
  },
  // LEFT HIND
  {
    {LEGLH, "LH"},
    { -LEG_BODY_X,  -LEG_BODY_Y_H,   LEG_BODY_Z},
    {-LEG_POINT_X, -LEG_POINT_Y_H, -LEG_POINT_Z},
    { LEG_SIZE_L1,    LEG_SIZE_L2,  LEG_SIZE_L3},
    {LEG_ANGLE_ALPHA_MIN, LEG_ANGLE_BETA_MIN, LEG_ANGLE_GAMMA_MIN},
    {LEG_ANGLE_ALPHA_MAX, LEG_ANGLE_BETA_MAX, LEG_ANGLE_GAMMA_MAX},
    {
      {M_PI_2, M_PI_4, M_PI_2},
      { 13, 12, 14},
      {  0,  0,  0},
      {  2,  1,  1}
    },
    {   -LEG_POINT_X, -LEG_POINT_Y_H,    -LEG_POINT_Z},
    {LEG_ANGLE_ALPHA, LEG_ANGLE_BETA, LEG_ANGLE_GAMMA},
    {true, true, false, true, false, true},
    {true, 0, 0}
  },
  // RIGHT HIND
  {
    {LEGRH, "RH"},
    { LEG_BODY_X,  -LEG_BODY_Y_H,   LEG_BODY_Z},
    {LEG_POINT_X, -LEG_POINT_Y_H, -LEG_POINT_Z},
    {LEG_SIZE_L1,    LEG_SIZE_L2,  LEG_SIZE_L3},
    {LEG_ANGLE_ALPHA_MIN, LEG_ANGLE_BETA_MIN, LEG_ANGLE_GAMMA_MIN},
    {LEG_ANGLE_ALPHA_MAX, LEG_ANGLE_BETA_MAX, LEG_ANGLE_GAMMA_MAX},
    {
      {M_PI_2, M_PI_4, M_PI_2},
      {  4,  2, 15},
      {  0,  0,  0},
      {  2,  1,  1}
    },
    {    LEG_POINT_X, -LEG_POINT_Y_H,    -LEG_POINT_Z},
    {LEG_ANGLE_ALPHA, LEG_ANGLE_BETA, LEG_ANGLE_GAMMA},
    {false, true, false, false, true, false},
    {true, 0, 0}
  }
};

/**
 * Not all servos are perfect
 * 
 * This is for TowerPro MG90D (or maybe I have clone, who knows)
 */
servoProfile servoMainProfile = {
    20,   // minAngle;
    160,  // maxAngle;
    780,  // degMin;
    1000, // deg30;
    1000, // deg50;
    1000, // deg70;
    1500, // deg90;
    1000, // deg110;
    1000, // deg130;
    1000, // deg150;
    2200  // degMax;
};

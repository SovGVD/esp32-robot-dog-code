#ifndef leg_h
#define leg_h

#include "geometry.h"
#define LEGLF 0   // left  front leg
#define LEGRF 1   // right front leg
#define LEGLH 2   // left  hind  leg
#define LEGRH 3   // right hind  leg

// see IK.h
typedef struct {
	double alpha;
	double beta;
	double gamma;
} legangle;

//Leg gears ratio
typedef struct {
	double alpha;
	double beta;
	double gamma;
} leggear;

// see IK.h
typedef struct {
	double l1;
	double l2;
	double l3;
} legsize;

// Hardware pin numbers
typedef struct {
	int alpha;
	int beta;
	int gamma;
} legpin;

// Leg sensor
typedef struct {
	bool onGround;  // State
	int pin;        // Settings
	int threshold;
} legsensor;

// Leg IK helpers (this is moslty need to set where is leg: left or right side, to inverse some values)
typedef struct {
	bool x;
	bool y;
	bool z;
	bool alpha;
	bool beta;
	bool gamma;
} leginverse;

// Leg HAL values
typedef struct {
	legangle mid;    // Hardware middle
	legpin   pin;
	legangle trim;
	leggear  ratio;
} leghal;

typedef struct {
	int  id;
	char title[4];
} legid;

// Leg structure
typedef struct {
	const legid     id;
	const point     defaultBody;
	const point     defaultFoot;
	const legsize   size;
	const legangle  min;	// setup limits of angles
	const legangle  max;	// setup limits of angles
	leghal          hal;
	point           body;
	point           foot;
	legangle        angle;
	leginverse      inverse;
	legsensor       sensor;
} leg;

#endif

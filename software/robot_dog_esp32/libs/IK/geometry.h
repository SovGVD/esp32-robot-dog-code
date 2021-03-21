#ifndef geometry_h
#define geometry_h

#define ALPHA 0
#define BETA  1
#define GAMMA 2

#define XAXIS     0
#define YAXIS     1
#define ZAXIS     2
#define PITCHAXIS 3
#define ROLLAXIS  4
#define YAWAXIS   5

// pount in 3D
typedef struct {
	double x;
	double y;
	double z;
} point;

// angle in 3D
typedef struct {
	double roll;    // X axis
	double pitch;   // Y axis
	double yaw;     // Z axis
} angle;

//body
typedef struct {
	point position;
	angle orientation;
} figure;

//move
typedef struct {
	point move;
	angle rotate;
} moveVector;


#endif

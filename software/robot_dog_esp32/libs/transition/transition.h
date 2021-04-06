/**
 * Transitions for leg movement
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */
 
// WARNING: this is WIP and this library was not tested in any software or hardware!!!
 
/**
 * transition of the leg is the set on two parabolic (TODO) functions and line on progress=[0,1];
 * at that moment I will use just a line
 * 
 *  Z
 *  ^
 *  |
 *  |
 *  | TODO parabolic func
 * 1|       .----------.
 *  |      /.          .\
 *  |    /  .          .  \
 *  |  /    .          .    \
 *  | /     .          .     \
 *  |/      .          .      \
 * 0+-------*----------*-------*----> XY (progress)
 *  0      0.2        0.8      1
 *
 *
 *
 *
 *   x - x1    y - y1    z - z1
 *  ------- = ------- = -------
 *  x2 - x1   y2 - y1   z2 - z1
 *
 *  {
 *    x = t*ax + x1;
 *        ax = x2 - x1;
 *
 *    y = t*(y2 - y1) + y1;
 *        ay = y2 - y1;
 *
 *    z = t*(z2 - z1) + z1;
 *        az = z2 - z1;
 *
 *    t = [0,1];
 *  }
 *
 *
 *  Z
 *  ^
 *  |
 *  |
 *  |     point1
 * 1|     *---___
 *  |    /.      ---*point2
 *  |   / .         . \
 *  |  /  .         .  \
 *  | /k1 .   k2    . k3\
 *  |/    .         .    \ targetValue
 * 0*-----|---------|-----*----> XY (progress)
 *  0    0.2        0.8    1
 * initialValue
 *
 * (yes, 0XY != 0Y, just as a sketch)
 * initialValue
 *  *-----|---------|-----*--->Y
 *  |\_   .         .
 *  |   \ _         .
 *  |     *\ _      .
 *  |    0.2  \ _   .      progressLength
 *  |            \ _.     .
 *  |               * _  .
 *  |             0.8  \ _
 *  v                     *
 * X                       1
 */
#ifndef transition_h
#define transition_h

#define TRANSITION_PROGRESS_STEP1 0.4
#define TRANSITION_PROGRESS_STEP2 0.4

typedef struct {
	figure initialValue;
	figure targetValue;
	double offTheGround;
} transitionParameters;

class transition
{
	public:
		transition();
		void set(transitionParameters param);
		figure swing(double progress);
		figure linear(double progress);
	private:
		transitionParameters _param;
		figure  f;
		double stepProgress;
		double dPx;
		double dPy;
		double dPz;
		double dOp;
		double dOr;
		double dOy;
		double z;
		double P_z1; // height off the ground on step1
		double P_z2; // height off the ground on step2
};

#endif

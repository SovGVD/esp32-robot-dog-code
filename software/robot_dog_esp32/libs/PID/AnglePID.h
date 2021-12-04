/**
 * PID
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2021
 */
 
#ifndef ANGLE_PID_h
#define ANGLE_PID_h

#include "../IK/geometry.h"

class AnglePID
{
	public:
		AnglePID(angle &current, angle &target, angle &output, double Kp, double Ki, double Kd, double minError, double maxError);
		void set(double Kp, double Ki, double Kd);
		void update();
	private:
		angle         *_current;
		angle         *_target;
		angle         *_output;
		double        _Kp = 0.0;
		double        _Ki = 0.0;
		double        _Kd = 0.0;
		double        _minError;
		double        _maxError;
		angle         P   = {0.0, 0.0, 0.0};
		angle         I   = {0.0, 0.0, 0.0};
		angle         D   = {0.0, 0.0, 0.0};
		angle         err = {0.0, 0.0, 0.0};
		angle         preCurrent = {0.0, 0.0, 0.0};
		angle         dInput = {0.0, 0.0, 0.0};
		angle         outputSum = {0.0, 0.0, 0.0};
		unsigned long _t  = 0;
		bool          pOnE = true;
};

#endif

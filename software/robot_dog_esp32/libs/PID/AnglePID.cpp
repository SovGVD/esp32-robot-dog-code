#include "math.h" 
#include "AnglePID.h"

AnglePID::AnglePID(angle &current, angle &target, angle &output, double Kp, double Ki, double Kd)
{
	_current = &current;
	_target  = &target;
	_output  = &output;

	_Kp = Kp;
	_Ki = Ki;
	_Kd = Kd;
}

void AnglePID::set(double Kp, double Ki, double Kd)
{
	_Kp = Kp;
	_Ki = Ki;
	_Kd = Kd;
}

void AnglePID::update()
{
	if (_t == 0) {
		_t = micros();
		return;
	}
	
	double dt = (micros() - _t) / 1000.0;
	
	P.pitch = _target->pitch - _current->pitch;
	P.roll  = _target->roll  - _current->roll;
	P.yaw   = _target->yaw   - _current->yaw;

	I.pitch = I.pitch + P.pitch * dt;
	I.roll  = I.roll  + P.roll  * dt;
	I.yaw   = I.yaw   + P.yaw   * dt;

	D.pitch = (P.pitch - err.pitch) / dt;
	D.roll  = (P.roll  - err.roll)  / dt;
	D.yaw   = (P.yaw   - err.yaw)   / dt;

	err.pitch = P.pitch;
	err.roll  = P.roll;
	err.yaw   = P.yaw;
	
	_output->pitch = P.pitch * _Kp + I.pitch * _Ki + D.pitch * _Kd;
	_output->roll  = P.roll  * _Kp + I.roll  * _Ki + D.roll  * _Kd;
	_output->yaw   = P.yaw   * _Kp + I.yaw   * _Ki + D.yaw   * _Kd;


	/*Serial.print(_current->pitch);
	Serial.print(",");
	Serial.print(_target->pitch);
	Serial.print(",");
	Serial.println(_output->pitch);*/
}

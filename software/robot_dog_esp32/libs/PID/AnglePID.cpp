#include "math.h"
#include "AnglePID.h"

AnglePID::AnglePID(angle &current, angle &target, angle &output, double Kp, double Ki, double Kd, double minError, double maxError)
{
	_current = &current;
	_target  = &target;
	_output  = &output;

	_Kp = Kp;
	_Ki = Ki;
	_Kd = Kd;
	
	_minError = minError;
	_maxError = maxError;
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
	
	err.pitch = _target->pitch - _current->pitch;
	err.roll  = _target->roll  - _current->roll;
	err.yaw   = _target->yaw   - _current->yaw;
	
	dInput.pitch = _current->pitch - preCurrent.pitch;
	dInput.roll  = _current->roll  - preCurrent.roll;
	dInput.yaw   = _current->yaw   - preCurrent.yaw;
	
	outputSum.pitch += _Ki * err.pitch;
	outputSum.roll  += _Ki * err.roll;
	outputSum.yaw   += _Ki * err.yaw;

	if (outputSum.pitch > _maxError)  outputSum.pitch = _maxError;
	if (outputSum.pitch < _minError)  outputSum.pitch = _minError;
	if (outputSum.roll  > _maxError)  outputSum.roll  = _maxError;
	if (outputSum.roll  < _minError)  outputSum.roll  = _minError;
	if (outputSum.yaw   > _maxError)  outputSum.yaw   = _maxError;
	if (outputSum.yaw   < _minError)  outputSum.yaw   = _minError;
	
	_output->pitch = _Kp * err.pitch + outputSum.pitch - _Kd * dInput.pitch;
	_output->roll  = _Kp * err.roll  + outputSum.roll  - _Kd * dInput.roll;
	_output->yaw   = _Kp * err.yaw   + outputSum.yaw   - _Kd * dInput.yaw;
	
	
	/*Serial.print(_current->pitch);
	Serial.print(",");
	Serial.print(_target->pitch);
	Serial.print(",");
	Serial.print(err.pitch);
	Serial.print(",");
	Serial.print(outputSum.pitch);
	Serial.print(",");
	Serial.println(_output->pitch);*/
	
	preCurrent.pitch = _current->pitch;
	preCurrent.roll  = _current->roll;
	preCurrent.yaw   = _current->yaw;
}

/**
 * Planner
 *
 * MIT Licence
 * Developed by Gleb Devyatkin (SovGVD) in 2020
 */
 // TODO use gait data to avoid unnecessary legs update
 // TODO do this with matrix and for 3D

#include "planner.h"

planner::planner(moveVector &vector, figure &bodyObj, leg &legLF, leg &legRF, leg &legLH, leg &legRH)
{
	_vector = &vector;
	_body   = &bodyObj;
	_legLF  = &legLF;
	_legRF  = &legRF;
	_legLH  = &legLH;
	_legRH  = &legRH;
}

void planner::predictPosition(uint8_t steps = 1)
{
	// TODO normalize angle !!! very important !!!
	_predictedBody.orientation.pitch = _body->orientation.pitch;
	_predictedBody.orientation.roll  = _body->orientation.roll;
	_predictedBody.orientation.yaw   = _body->orientation.yaw + rotateInc * _vector->rotate.yaw;
	
	
	double tmpSin = sin(_predictedBody.orientation.yaw);
	double tmpCos = cos(_predictedBody.orientation.yaw);
	
	// TODO use matrix
	_predictedBody.position.x = _body->position.x + moveInc * (_vector->move.x * tmpCos - _vector->move.y * tmpSin);
	_predictedBody.position.y = _body->position.y + moveInc * (_vector->move.x * tmpSin + _vector->move.y * tmpCos);
	_predictedBody.position.z = _body->position.z;
	
	
	// This is terible (code)
	// Im trying to get new position of legs based on rotation of default position for XY-plane
	// this is OK for first time, but terrible for anything else

	_predictedLegLFfoot.x = _predictedBody.position.x + _legLF->defaultFoot.x * tmpCos - _legLF->defaultFoot.y * tmpSin;
	_predictedLegLFfoot.y = _predictedBody.position.y + _legLF->defaultFoot.x * tmpSin + _legLF->defaultFoot.y * tmpCos;
	_predictedLegLFfoot.z = _legLF->defaultFoot.z;

	_predictedLegRFfoot.x = _predictedBody.position.x + _legRF->defaultFoot.x * tmpCos - _legRF->defaultFoot.y * tmpSin;
	_predictedLegRFfoot.y = _predictedBody.position.y + _legRF->defaultFoot.x * tmpSin + _legRF->defaultFoot.y * tmpCos;
	_predictedLegRFfoot.z = _legRF->defaultFoot.z;

	_predictedLegLHfoot.x = _predictedBody.position.x + _legLH->defaultFoot.x * tmpCos - _legLH->defaultFoot.y * tmpSin;
	_predictedLegLHfoot.y = _predictedBody.position.y + _legLH->defaultFoot.x * tmpSin + _legLH->defaultFoot.y * tmpCos;
	_predictedLegLHfoot.z = _legLH->defaultFoot.z;

	_predictedLegRHfoot.x = _predictedBody.position.x + _legRH->defaultFoot.x * tmpCos - _legRH->defaultFoot.y * tmpSin;
	_predictedLegRHfoot.y = _predictedBody.position.y + _legRH->defaultFoot.x * tmpSin + _legRH->defaultFoot.y * tmpCos;
	_predictedLegRHfoot.z = _legRH->defaultFoot.z;
}

figure planner::getBodyPosition()
{
	return _predictedBody;
}

point planner::getLegPosition(uint8_t legId)
{
	// this is looks so dump
	switch(legId) {
		case LEGLF: return _predictedLegLFfoot;
		case LEGRF: return _predictedLegRFfoot;
		case LEGLH: return _predictedLegLHfoot;
		case LEGRH: return _predictedLegRHfoot;
	}
	
	// i hope it will never happens
	return {0,0,0};
}

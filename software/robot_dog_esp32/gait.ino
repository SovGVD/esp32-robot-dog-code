// Gait
gaitConfig GAIT_CONFIG = {
  {
    { SWING,  STANCE, STANCE, SWING  },
    { STANCE, SWING,  SWING,  STANCE }
  },
  2,          // items in sequence
  LOOP_TIME,  // loop time in microseconds to correclty calculate number of sub moves per gate item
  30,         // above the ground on swing, 25mm
  100000,     // swing duration, microseconds
  200000      // sequence items duration, microseconds
};

gait gaitLegLF(GAIT_CONFIG, legs[LEGLF]);
gait gaitLegRF(GAIT_CONFIG, legs[LEGRF]);
gait gaitLegLH(GAIT_CONFIG, legs[LEGLH]);
gait gaitLegRH(GAIT_CONFIG, legs[LEGRH]);


void initGait() {
  ticksPerGaitItem = GAIT_CONFIG.duration/GAIT_CONFIG.loopTime;
  ticksToNextGaitItem = ticksPerGaitItem;
}

uint8_t getNextGait()
{
  nextGait = currentGait + 1;
  if (nextGait >= GAIT_CONFIG.sequenceLength) nextGait = 0;
  return nextGait;
}

void updateGait() {
  ticksToNextGaitItem--;

  gaitItemProgress = 1 - (float)ticksToNextGaitItem/(float)ticksPerGaitItem;

  figure transitionProgress = bodyTransition.linear(gaitItemProgress);
  body.position = transitionProgress.position;
  body.orientation = transitionProgress.orientation;


// TODO not the best place for calculation of body leg points
  double tmpSin = sin(body.orientation.yaw);
  double tmpCos = cos(body.orientation.yaw);

  for (int i = 0; i < LEG_NUM; i++) {
    legs[i].body.x = body.position.x + legs[i].defaultBody.x * tmpCos - legs[i].defaultBody.y * tmpSin;
    legs[i].body.y = body.position.y + legs[i].defaultBody.x * tmpSin + legs[i].defaultBody.y * tmpCos;
  }

  gaitProgress[LEGLF] = gaitLegLF.next();
  gaitProgress[LEGRF] = gaitLegRF.next();
  gaitProgress[LEGLH] = gaitLegLH.next();
  gaitProgress[LEGRH] = gaitLegRH.next();


  // TODO we need predict next position of robot to begin move of CoM at 0.8 progress of current and until 0.2 progress of future gait
  //bodyBalance.setBody(bodyBalance.getCenter());

  if (ticksToNextGaitItem <= 0) {
    ticksToNextGaitItem = ticksPerGaitItem;
    currentGait++;
    if (currentGait >= GAIT_CONFIG.sequenceLength) currentGait = 0;

    // set future position - this needs to be done on 0.8 progress for CoM and balance transition, that is also should include body linear transition
    walkPlanner.predictPosition();

    // body linear transition (TODO, include balance here)
    bodyTransitionParams.initialValue = body;
    bodyTransitionParams.targetValue  = walkPlanner.getBodyPosition();
    bodyTransition.set(bodyTransitionParams);

    // run gait workers
    for (int i = 0; i < LEG_NUM; i++) {
      if (GAIT_CONFIG.sequence[currentGait].leg[i] == SWING) {
        switch(i) {
          case LEGLF:
            gaitLegLF.start(legs[i].foot, walkPlanner.getLegPosition(i));
            break;
          case LEGRF:
            gaitLegRF.start(legs[i].foot, walkPlanner.getLegPosition(i));
            break;
          case LEGLH:
            gaitLegLH.start(legs[i].foot, walkPlanner.getLegPosition(i));
            break;
          case LEGRH:
            gaitLegRH.start(legs[i].foot, walkPlanner.getLegPosition(i));
            break;
        }
      }
    }
  }
}

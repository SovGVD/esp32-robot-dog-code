// Gait
gaitConfig GAIT_CONFIG = {
  {
    { SWING,  STANCE, STANCE, SWING  },
    { STANCE, SWING,  SWING,  STANCE }
  },
  2,           // items in sequence
  LOOP_TIME,   // loop time to correclty calculate number of sub moves per gate item
  40,          // above the ground on swing, 40mm
  200,         // swing duration
  200          // sequence items duration
};

/*
gaitConfig GAIT_CONFIG = {
  {
    {SWING,  STANCE, STANCE, STANCE},
    {STANCE, STANCE, SWING,  STANCE},
    {STANCE, SWING,  STANCE, STANCE},
    {STANCE, STANCE, STANCE, SWING }
  },
  4,           // items in sequence
  LOOP_TIME,   // loop time to correclty calculate number of sub moves per gate item
  30,          // above the ground on swing, 30mm
  2000,         // swing duration
  2000          // sequence items duration
};*/

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

  body.position = bodyTransition.linear(gaitItemProgress);
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
    bodyTransitionParams.initialValue = body.position;
    bodyTransitionParams.targetValue  = walkPlanner.getBodyPosition().position;
    bodyTransition.set(bodyTransitionParams);

    // run gait workers
    for (int i = 0; i < LEG_NUM; i++) {
      if (GAIT_CONFIG.sequence[currentGait].leg[i] == SWING) {
        switch(i) {
          case LEGLF: gaitLegLF.start(legs[i].foot, walkPlanner.getLegPosition(i)); break;
          case LEGRF: gaitLegRF.start(legs[i].foot, walkPlanner.getLegPosition(i)); break;
          case LEGLH: gaitLegLH.start(legs[i].foot, walkPlanner.getLegPosition(i)); break;
          case LEGRH: gaitLegRH.start(legs[i].foot, walkPlanner.getLegPosition(i)); break;
        }
      }
    }
  }
}

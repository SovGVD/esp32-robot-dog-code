# Small Robot dog (quadruped)

![Small robot dog](https://github.com/SovGVD/esp32-robot-dog-code/blob/master/assets/img/small.jpg?raw=true)

## Hardware
- ESP32
- IMU (not implemented)
- 12 servos TowerPro mg90d
- Two 18650

## Software
- Arduino IDE compatible

## TODO
- [x] use IMU for leveling
- [ ] UI, CLI, EEPROM and docs for IMU leveling
- [ ] use power sensor for something

## How to
### Calibrate servos (create `servoMainProfile`)
- Print servo_calib tool and install servo into it: circle plate with dots, 10deg each from 0 to 180.
- use tools/servoCalib.ino and connect servo to 14 pin
- open Arduino IDE terminal and input `1500` (and press Enter) - this should be servo middle and it should point to the middle dot of printed tool
- decrease value to find `minAngle` and `degMin` values for it (start with `800` and decrease it until servo stop move, than return back one step, e.g. set 790 - servo moved, 780 - servo moved, 760 - servo don't move, use 780)
- do the same to find `maxAngle` and `degMax` but make value and start from 2100 and increase values
- great, now we know our servo limits (or at least what are the limits for lib+servo), time to find more accurate servo positions
- input values until you will not find proper positions for deg30, deg50...deg130, deg150

### Legs
#### Assembling
- to assembly legs correctly print leg_calib tool/template one as it is and one mirrored for the other side of robot for Beta and Gamma angles, as also Alpha angle tool
- power up servo and connect ESP32 to you computer, open Arduino IDE terminal
- input `set servo_to_calib` to set all servo to position expected for printer tool
- assemble legs as closer as possible to expected leg parts positions according to the tool (90deg, 45deg, 90deg)

#### Calibration
- repeat steps 2,3 of Legs->Assembling instruction
- input `set help` to see the list of available commands, we are interested in `XX_HAL_trim_xxxx`, e.g. `LF_HAL_trim_alpha`, where `LF` - left front leg, and `alpha` is the angle name
- put Alpha leg tool on top of the robot legs servo, surface of tool should be (near)perfectly align with servos body, if not, use `XX_HAL_trim_alpha value_in_deg` command to set servo trim value, e.g. `set LF_HAL_trim_alpha -3`, it should not be too big, in other cases you need to repeat `Assemble` step
- using tool for Beta and Gamma angles, calibrate/trim other servos

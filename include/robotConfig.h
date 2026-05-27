#pragma once

#include "include/motor.h"

typedef struct
{
  Motor frontLeftMotor;
  Motor frontRightMotor;
  Motor backLeftMotor;
  Motor backRightMotor;
} RobotConfig;

static RobotConfig robotConfig = {
    .frontLeftMotor = {.pinA = 2, .pinB = 3, .reversed = false},
    .frontRightMotor = {.pinA = 6, .pinB = 7, .reversed = false},
    .backRightMotor = {.pinA = 18, .pinB = 19, .reversed = false},
    .backLeftMotor = {.pinA = 20, .pinB = 21, .reversed = false},
};
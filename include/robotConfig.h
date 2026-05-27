#pragma once

#include "motor.h"

static Motor frontLeftMotor = {.pinA = 2, .pinB = 3, .reversed = false};
static Motor frontRightMotor = {.pinA = 6, .pinB = 7, .reversed = false};
static Motor backRightMotor = {.pinA = 18, .pinB = 19, .reversed = false};
static Motor backLeftMotor = {.pinA = 20, .pinB = 21, .reversed = false};
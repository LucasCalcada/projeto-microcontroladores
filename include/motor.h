#pragma once

#include "pico/types.h"

#define WRAP 12499
#define DEADZONE 1000
#define MAX_SPEED (WRAP / 2)

typedef struct
{
  uint pinA;
  uint pinB;
  bool reversed;
  int currentSpeed;
} Motor;

void motor_setup(Motor *motor);
void motor_move(Motor *motor, int speed);

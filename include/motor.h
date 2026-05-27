#pragma once

#include "pico/types.h"

typedef struct
{
  uint pinA;
  uint pinB;
  bool reversed;
} Motor;

void motor_setup(Motor motor);
void motor_move(Motor motor, int speed);

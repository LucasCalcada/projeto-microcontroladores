#pragma once

#include "8Bitdo_payload.h"
#include "motor.h"
#include <math.h>

typedef struct
{
  int frontLeft;
  int frontRight;
  int backLeft;
  int backRight;
} Movement;

static float normalize(uint8_t v)
{
  return ((float)v - 128.0f) / 128.0f;
}

Movement OmniMovement(Joystick inputL, Joystick inputR)
{
  Movement m;

  float Vx = normalize(inputL.x);
  float Vy = normalize(inputL.y);
  float omega = normalize(inputR.x);

  float fl = Vy + Vx + omega;
  float fr = Vy - Vx - omega;
  float bl = Vy - Vx + omega;
  float br = Vy + Vx - omega;

  // normalização (evita saturação)
  float max = fabsf(fl);

  if (fabsf(fr) > max)
    max = fabsf(fr);
  if (fabsf(bl) > max)
    max = fabsf(bl);
  if (fabsf(br) > max)
    max = fabsf(br);

  if (max > 1.0f)
  {
    fl /= max;
    fr /= max;
    bl /= max;
    br /= max;
  }

  // escala final para inteiro
  m.frontLeft = fl * MAX_SPEED;
  m.frontRight = fr * MAX_SPEED;
  m.backLeft = bl * MAX_SPEED;
  m.backRight = br * MAX_SPEED;

  return m;
}
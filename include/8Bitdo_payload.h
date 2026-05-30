#pragma once

#include "pico/stdlib.h"
#include <stdio.h>

typedef struct
{
  uint8_t x;
  uint8_t y;
} Joystick;

typedef enum
{
  Up = 0b0000 << 4,
  UpRight = 0b1000 << 4,
  Right = 0b0100 << 4,
  BottomRight = 0b1100 << 4,
  Bottom = 0b0010 << 4,
  BottomLeft = 0b1010 << 4,
  Left = 0b0110 << 4,
  UpLeft = 0b1110 << 4,
  Neutral = 0b1111 << 4
} DpadDirections;

typedef enum
{
  Btn_A = 1 << 16,
  Btn_B = 1 << 14,
  Btn_L4 = 1 << 13,
  Btn_X = 1 << 12,
  Btn_Y = 1 << 11,
  Btn_R4 = 1 << 10,
  Btn_L1 = 1 << 9,
  Btn_R1 = 1 << 8,
  Btn_L2 = 1 << 7,
  Btn_R2 = 1 << 6,
  Btn_Minus = 1 << 5,
  Btn_Plus = 1 << 4,
  Btn_Main = 1 << 3,
  Btn_L3 = 1 << 2,
  Btn_R3 = 1 << 1,
} ButtonMapping;

typedef struct __attribute__((packed))
{
  uint8_t dpad;
  Joystick left;
  Joystick right;
  uint8_t triggerRight;
  uint8_t triggerLeft;
  uint16_t buttons;
} Controller_Payload;
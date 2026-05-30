#pragma once

#include "pico/stdlib.h"
#include "8Bitdo_payload.h"

typedef enum
{
  CONTROLLER_DISCONNECTED,
  CONTROLLER_CONNECTED,
  CONTROLLER_SCANNING,
} BtControllerStatus;

typedef struct
{
  uint8_t addr[6];
  BtControllerStatus status;
  Controller_Payload payload;
} BtControllerConfig;

void bluetooth_setup(BtControllerConfig *config);
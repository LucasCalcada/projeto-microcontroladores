#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include "motor.h"
#include "robotConfig.h"
#include "bluetooth_controller.h"

BtControllerConfig config = {
    .addr = {0xe4, 0x17, 0xd8, 0x37, 0x73, 0x23},
    .status = CONTROLLER_DISCONNECTED};

void input_handler(const Controller_Payload *payload)
{
  printf("Joystick L: %2X %2X\n", payload->left.x, payload->left.y);
}

int main()
{
  stdio_init_all();
  bluetooth_register_handler(input_handler);
  bluetooth_setup(&config);
}

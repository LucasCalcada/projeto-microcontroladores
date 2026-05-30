#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include "motor.h"
#include "robotConfig.h"
#include "bluetooth_controller.h"

BtControllerConfig config = {
    .addr = {0xe4, 0x17, 0xd8, 0x37, 0x73, 0x23},
    .status = CONTROLLER_DISCONNECTED};

int main()
{
  stdio_init_all();
  bluetooth_setup(&config);
}

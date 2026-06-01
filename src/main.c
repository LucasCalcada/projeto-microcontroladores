#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include "motor.h"
#include "movement.h"
#include "bluetooth_controller.h"

Motor frontLeftMotor = {.pinA = 3, .pinB = 4, .reversed = false};
Motor frontRightMotor = {.pinA = 6, .pinB = 7, .reversed = true};
Motor backLeftMotor = {.pinA = 12, .pinB = 13, .reversed = true};
Motor backRightMotor = {.pinA = 10, .pinB = 11, .reversed = true};

BtControllerConfig config = {
    .addr = {0xe4, 0x17, 0xd8, 0x37, 0x73, 0x23},
    .status = CONTROLLER_DISCONNECTED};

void input_handler(const Controller_Payload *payload)
{
  Movement m = OmniMovement(payload->left, payload->right);

  motor_move(&frontLeftMotor, m.frontLeft);
  motor_move(&backLeftMotor, m.backLeft);
  motor_move(&frontRightMotor, m.frontRight);
  motor_move(&backRightMotor, m.backRight);
}

int main()
{
  stdio_init_all();

  motor_setup(&frontLeftMotor);
  motor_setup(&backLeftMotor);
  motor_setup(&frontRightMotor);
  motor_setup(&backRightMotor);

  bluetooth_register_handler(input_handler);
  bluetooth_setup(&config);
}

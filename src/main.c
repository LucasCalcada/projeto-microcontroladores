#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include "motor.h"
#include "robotConfig.h"

int main()
{
  motor_setup(frontLeftMotor);
  motor_setup(frontRightMotor);
  motor_setup(backLeftMotor);
  motor_setup(backRightMotor);

  while (true)
  {
    motor_move(frontLeftMotor, 1000);
    sleep_ms(500);
    motor_move(frontLeftMotor, 0);

    motor_move(frontRightMotor, 1000);
    sleep_ms(500);
    motor_move(frontRightMotor, 0);

    motor_move(backLeftMotor, 1000);
    sleep_ms(500);
    motor_move(backLeftMotor, 0);

    motor_move(backRightMotor, 1000);
    sleep_ms(500);
    motor_move(backRightMotor, 0);

    motor_move(frontLeftMotor, -1000);
    sleep_ms(500);
    motor_move(frontLeftMotor, 0);

    motor_move(frontRightMotor, -1000);
    sleep_ms(500);
    motor_move(frontRightMotor, 0);

    motor_move(backLeftMotor, -1000);
    sleep_ms(500);
    motor_move(backLeftMotor, 0);

    motor_move(backRightMotor, -1000);
    sleep_ms(500);
    motor_move(backRightMotor, 0);
  }
}

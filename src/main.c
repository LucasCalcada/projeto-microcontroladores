#include "hardware/gpio.h"
#include "hardware/timer.h"

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
    gpio_put(PICO_DEFAULT_LED_PIN, true);
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    sleep_ms(500);
  }
}

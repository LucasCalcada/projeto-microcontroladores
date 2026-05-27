#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "include/robotConfig.h"

int main()
{
  motor_setup(robotConfig.frontLeftMotor);
  motor_setup(robotConfig.frontRightMotor);
  motor_setup(robotConfig.backLeftMotor);
  motor_setup(robotConfig.backRightMotor);

  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  while (true)
  {
    gpio_put(PICO_DEFAULT_LED_PIN, true);
    sleep_ms(500);
    gpio_put(PICO_DEFAULT_LED_PIN, false);
    sleep_ms(500);
  }
}

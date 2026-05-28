#include "motor.h"

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdlib.h>

void motor_setup(Motor motor)
{
  gpio_init(motor.pinA);
  gpio_set_function(motor.pinA, GPIO_FUNC_PWM);

  gpio_init(motor.pinB);
  gpio_set_function(motor.pinB, GPIO_FUNC_PWM);

  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 1.0f);
  pwm_config_set_wrap(&config, 2000);
  uint slice = pwm_gpio_to_slice_num(motor.pinA);
  pwm_init(slice, &config, true);
}

void motor_move(Motor motor, int speed)
{
  if (speed == 0)
  {
    pwm_set_gpio_level(motor.pinA, 0);
    pwm_set_gpio_level(motor.pinB, 0);
    return;
  }

  uint highPin = speed > 0 ? motor.pinA : motor.pinB;
  uint lowPin = speed > 0 ? motor.pinB : motor.pinA;

  if (motor.reversed)
  {
    uint temp = lowPin;
    lowPin = highPin;
    highPin = temp;
  }

  pwm_set_gpio_level(highPin, abs(speed));
  pwm_set_gpio_level(lowPin, 0);
}
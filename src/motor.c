#include "motor.h"

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include <stdlib.h>
#include "pico/stdlib.h"

#define DEADTIME_US 50

void motor_setup(Motor *motor)
{
  gpio_init(motor->pinA);
  gpio_set_function(motor->pinA, GPIO_FUNC_PWM);

  gpio_init(motor->pinB);
  gpio_set_function(motor->pinB, GPIO_FUNC_PWM);

  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 1.0f);
  pwm_config_set_wrap(&config, WRAP);

  motor->currentSpeed = 0;

  uint sliceA = pwm_gpio_to_slice_num(motor->pinA);
  pwm_init(sliceA, &config, true);
  pwm_set_gpio_level(motor->pinA, 0);

  uint sliceB = pwm_gpio_to_slice_num(motor->pinB);
  pwm_init(sliceB, &config, true);
  pwm_set_gpio_level(motor->pinB, 0);
}

static int parse_speed(const Motor *motor, int speed)
{
  if (abs(speed) <= DEADZONE)
    return 0;

  if (speed > MAX_SPEED)
    speed = MAX_SPEED;

  if (speed < -MAX_SPEED)
    speed = -MAX_SPEED;

  if (motor->reversed)
  {
    return -speed;
  }

  return speed;
}

void dead_time(Motor *motor, int speed)
{
  int old_speed = motor->currentSpeed;
  bool old_dir = old_speed >= 0;
  bool new_dir = speed >= 0;

  if (old_speed != 0 && old_dir != new_dir)
  {
    pwm_set_gpio_level(motor->pinA, 0);
    pwm_set_gpio_level(motor->pinB, 0);

    sleep_us(DEADTIME_US);
  }
}

void motor_move(Motor *motor, int speed)
{
  speed = parse_speed(motor, speed);

  dead_time(motor, speed);

  if (speed > 0)
  {
    pwm_set_gpio_level(motor->pinB, 0);
    pwm_set_gpio_level(motor->pinA, speed);
  }
  else if (speed < 0)
  {
    pwm_set_gpio_level(motor->pinA, 0);
    pwm_set_gpio_level(motor->pinB, -speed);
  }
  else
  {
    pwm_set_gpio_level(motor->pinA, 0);
    pwm_set_gpio_level(motor->pinB, 0);
  }

  motor->currentSpeed = speed;
}
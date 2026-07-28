#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>
#include <math.h>

#define Left_Front_RPWM_PIN 11 // Arduino PWM output pin 13
#define Left_Front_LPWM_PIN 10 // Arduino PWM output pin 12
//Hacia el puente H el RPWM va al LPWM del puente. y LPWM a RPWM
#define Right_Front_RPWM_PIN 9 // Arduino PWM output pin 11
#define Right_Front_LPWM_PIN 8 // Arduino PWM output pin 10

#define Left_Rear_RPWM_PIN 7 // Arduino PWM output pin 13
#define Left_Rear_LPWM_PIN 6 // Arduino PWM output pin 12
//Hacia el puente H el RPWM va al LPWM del puente. y LPWM a RPWM
#define Right_Rear_RPWM_PIN 5 // Arduino PWM output pin 11
#define Right_Rear_LPWM_PIN 4 // Arduino PWM output pin 10


typedef enum actions{
  Backward = -1,
  Stop = 0,
  Forward = 1,
  Left = 2,
  Right = 3,
  Left_Rotation = 4,
  Right_Rotation = 5,
};

void f_init_motors(void);
void f_update_motors_cmd(int duty_cycle, int *cmd);


#endif

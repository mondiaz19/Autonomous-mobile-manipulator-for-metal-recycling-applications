
#include "control.h"

uint8_t PWM = 0;

void f_init_motors(void) {
  pinMode(Left_Front_RPWM_PIN, OUTPUT);
  pinMode(Left_Front_LPWM_PIN, OUTPUT);
  pinMode(Right_Front_RPWM_PIN, OUTPUT);
  pinMode(Right_Front_LPWM_PIN, OUTPUT);

  pinMode(Left_Rear_RPWM_PIN, OUTPUT);
  pinMode(Left_Rear_LPWM_PIN, OUTPUT);
  pinMode(Right_Rear_RPWM_PIN, OUTPUT);
  pinMode(Right_Rear_LPWM_PIN, OUTPUT);

  Serial.println("Motors Init Done");
}

void f_update_motors_cmd(int duty_cycle, int *cmd) {
  PWM = uint8_t(floor(255 * duty_cycle / 100));
  switch (*cmd) {
    case Forward:
      Serial.println("Forward");
      analogWrite(Left_Front_RPWM_PIN, floor(PWM / 5));
      analogWrite(Left_Front_LPWM_PIN, 0);
      analogWrite(Right_Front_RPWM_PIN, floor(PWM / 5));
      analogWrite(Right_Front_LPWM_PIN, 0);

      analogWrite(Left_Rear_RPWM_PIN, PWM);
      analogWrite(Left_Rear_LPWM_PIN, 0);
      analogWrite(Right_Rear_RPWM_PIN, PWM);
      analogWrite(Right_Rear_LPWM_PIN, 0);
      break;
    case Right:
      Serial.println("Right");
      analogWrite(Left_Front_RPWM_PIN, floor(PWM / 5));
      analogWrite(Left_Front_LPWM_PIN, 0);
      analogWrite(Right_Front_RPWM_PIN, 0);
      analogWrite(Right_Front_LPWM_PIN, floor(PWM / 5));

      analogWrite(Left_Rear_RPWM_PIN, 0);
      analogWrite(Left_Rear_LPWM_PIN, PWM);
      analogWrite(Right_Rear_RPWM_PIN, PWM);
      analogWrite(Right_Rear_LPWM_PIN, 0);
      break;
    case Left:
      Serial.println("Left");
      analogWrite(Left_Front_RPWM_PIN, 0);
      analogWrite(Left_Front_LPWM_PIN, floor(PWM / 5));
      analogWrite(Right_Front_RPWM_PIN, floor(PWM / 5));
      analogWrite(Right_Front_LPWM_PIN, 0);

      analogWrite(Left_Rear_RPWM_PIN, PWM);
      analogWrite(Left_Rear_LPWM_PIN, 0);
      analogWrite(Right_Rear_RPWM_PIN, 0);
      analogWrite(Right_Rear_LPWM_PIN, PWM);
      break;
    case Backward:
      Serial.println("Backward");
      analogWrite(Left_Front_RPWM_PIN, 0);
      analogWrite(Left_Front_LPWM_PIN, floor(PWM / 5));
      analogWrite(Right_Front_RPWM_PIN, 0);
      analogWrite(Right_Front_LPWM_PIN, floor(PWM / 5));

      analogWrite(Left_Rear_RPWM_PIN, 0);
      analogWrite(Left_Rear_LPWM_PIN, PWM);
      analogWrite(Right_Rear_RPWM_PIN, 0);
      analogWrite(Right_Rear_LPWM_PIN, PWM);
      break;
    case Right_Rotation:
      Serial.println("Right_Rotation");
      analogWrite(Left_Front_RPWM_PIN, floor(PWM / 5));
      analogWrite(Left_Front_LPWM_PIN, 0);
      analogWrite(Right_Front_RPWM_PIN, 0);
      analogWrite(Right_Front_LPWM_PIN, floor(PWM / 5));

      analogWrite(Left_Rear_RPWM_PIN, PWM);
      analogWrite(Left_Rear_LPWM_PIN, 0);
      analogWrite(Right_Rear_RPWM_PIN, 0);
      analogWrite(Right_Rear_LPWM_PIN, PWM);
      break;
    case Left_Rotation:
      Serial.println("Left_Rotation");
      analogWrite(Left_Front_RPWM_PIN, 0);
      analogWrite(Left_Front_LPWM_PIN, floor(PWM / 5));
      analogWrite(Right_Front_RPWM_PIN, floor(PWM / 5));
      analogWrite(Right_Front_LPWM_PIN, 0);

      analogWrite(Left_Rear_RPWM_PIN, 0);
      analogWrite(Left_Rear_LPWM_PIN, PWM);
      analogWrite(Right_Rear_RPWM_PIN, PWM);
      analogWrite(Right_Rear_LPWM_PIN, 0);
      break;
    default:
      analogWrite(Left_Front_RPWM_PIN, 0);
      analogWrite(Left_Front_LPWM_PIN, 0);
      analogWrite(Right_Front_RPWM_PIN, 0);
      analogWrite(Right_Front_LPWM_PIN, 0);

      analogWrite(Left_Rear_RPWM_PIN, 0);
      analogWrite(Left_Rear_LPWM_PIN, 0);
      analogWrite(Right_Rear_RPWM_PIN, 0);
      analogWrite(Right_Rear_LPWM_PIN, 0);
      break;
  }
  delay(250);
  *cmd = 0;
}

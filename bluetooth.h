#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>
#include <string.h>

//#define TX_BT_PIN 14 // Arduino
//#define RX_BT_PIN 15 // Arduino

void f_init_bth(void);
void f_read_bth(int *duty_cycle, int *cmd);

#endif

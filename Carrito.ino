
#include "control.h"
#include "bluetooth.h"

int duty_cycle;
int cmd; 


void setup()
{
  Serial.begin(9600);    // Inicializamos  el puerto serie  
  delay(100);
  f_init_motors();
  f_init_bth();
}

void loop()
{
  
  f_read_bth(&duty_cycle, &cmd);
  f_update_motors_cmd(duty_cycle, &cmd);

  /*if(Serial.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  {
    Serial7.write(Serial.read());
  }*/


}

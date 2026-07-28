
#include "bluetooth.h"

char data;

String message;
uint8_t my_index, my_index2;
uint8_t count_BT = 0;

void f_init_bth(void)
{
  //Serial3 ARDUINO - Serial7 TIVA
  Serial3.begin(38400);  // Inicializamos el puerto serie BT (Para Modo AT 2)
  delay(500);
  Serial.println("Enter AT commands:"); 
}

void f_read_bth(int *duty_cycle, int *cmd)
{
  while(Serial3.available()) // Si llega un dato por el puerto BT se envía al monitor serial
  {   
    data = Serial3.read();
    count_BT++;
    message.concat(data);
  }

  if((Serial3.available() == 0) && (count_BT >= 4)) 
  {
    //Serial.println(message);
    my_index = message.indexOf(',');
    my_index2 = message.indexOf('.');
    //Serial.println(index);
    
    *cmd = message.substring(0, my_index).toInt();
    Serial.println(*cmd);

    *duty_cycle = message.substring(my_index+1, my_index2).toInt();
    Serial.println(*duty_cycle);

    count_BT = 0;
    message = "";
  }
}

#include "control.h"
#include "bluetooth.h"

int duty_cycle;
int cmd; 

String n;
String a1;
String a2;
String b1;
String b2;
float a;
float b;
int i1,i2,i3,i4;

//int m1 = 2;
//int m2 = 3;

float FuzT(float u, float a, float b, float c);
float FuzZ(float u, float a, float b);
float FuzS(float u, float a, float b);

///////////////////////////////////////// X ///////////////////////////////////////////////

//Membership functions for Delta_X
float C1_MF1_In1[3]={-1000,-500};    //Cuadro
float C1_MF2_In1[4]={-1000,-500,0};  //trian
float C1_MF3_In1[4]={-500,0,500};   //trian 0
float C1_MF4_In1[4]={0,500,1000};    //trian
float C1_MF5_In1[3]={500,1000};      //Cuadro
// Delta punto (velocidad)
float C1_MF1_In2[3]={-1000,-500};
float C1_MF2_In2[4]={-1000,-500,0};
float C1_MF3_In2[4]={-500,0,500};
float C1_MF4_In2[4]={0,500,1000};
float C1_MF5_In2[3]={500,1000};

float C1_Out[25] = {-10,-5,-2,-1, 0, -5 ,-2,-1, 0, 1, -2 ,-1, 0, 1, 2, -1 , 0, 1, 2, 5, 0 , 1, 2, 5, 10};
float C1_I[25];
float C1_I1[5];
float C1_I2[5];

float in11, in12;
float ain11, ain21;
float out1;

///////////////////////////////////////// Y ///////////////////////////////////////////////

//Membership functions for Delta_Y
float C2_MF1_In1[3]={-200,-100};    //Cuadro
float C2_MF2_In1[4]={-200,-100,0};  //trian
float C2_MF3_In1[4]={-100,0,100};   //trian 0
float C2_MF4_In1[4]={0,100,200};    //trian
float C2_MF5_In1[3]={100,200};      //Cuadro
// Delta punto (velocidad)
float C2_MF1_In2[3]={-200,-100};
float C2_MF2_In2[4]={-200,-100,0};
float C2_MF3_In2[4]={-100,0,100};
float C2_MF4_In2[4]={0,100,200};
float C2_MF5_In2[3]={100,200};

float C2_Out[25] = {-10,-5,-2,-1, 0, -5 ,-2,-1, 0, 1, -2 ,-1, 0, 1, 2, -1 , 0, 1, 2, 5, 0 , 1, 2, 5, 10};
float C2_I[25];
float C2_I1[5];
float C2_I2[5];

float in21, in22;
float out2;


void setup() {
  Serial.begin(9600);
//  pinMode(m1, OUTPUT);
//  pinMode(m2, OUTPUT);
  delay(100);
  f_init_bth(); 
  f_init_motors();

  
  in11 = 0.0; // Delta x
  in12 = 0.0; // Delta punto x

  in21 = 0.0; // Delta x
  in22 = 0.0; // Delta punto x

}

void loop() {
    // put your main code here, to run repeatedly:
  if (Serial3.available() > 0) {
    n = Serial3.readStringUntil('\n');
//Serial.println(n);
  i1= n.indexOf('m');
  i2= n.indexOf('n');
  i3= n.indexOf('o');
  i4= n.indexOf('p');
  a1 = n.substring(i1+1,i2);
  in11 = a1.toFloat();
  b1 = n.substring(i2+1);
  in12 = b1.toFloat();
  a2 = n.substring(i3+1,i4);
  in21 = a2.toFloat();
  b2 = n.substring(i4+1);
  in22 = b2.toFloat();

  in11= in11*100; 
  in12= in12*100;
  in21= in21*100;
  in22= in22*100;
  
  }
  // Outputs
  out1 = 0.0;
  out2 = 0.0;

  // Evaluación de funciones de membresía para Delta X
  C1_I1[0] = FuzZ(in11, C1_MF1_In1[0], C1_MF1_In1[1]);
  C1_I1[1] = FuzT(in11, C1_MF2_In1[0], C1_MF2_In1[1], C1_MF2_In1[2]);
  C1_I1[2] = FuzT(in11, C1_MF3_In1[0], C1_MF3_In1[1], C1_MF3_In1[2]);
  C1_I1[3] = FuzT(in11, C1_MF4_In1[0], C1_MF4_In1[1], C1_MF4_In1[2]);
  C1_I1[4] = FuzS(in11, C1_MF5_In1[0], C1_MF5_In1[1]);

  // Evaluación de funciones de membresía para Delta Punto X
  C1_I2[0] = FuzZ(in12, C1_MF1_In2[0], C1_MF1_In2[1]);
  C1_I2[1] = FuzT(in12, C1_MF2_In2[0], C1_MF2_In2[1], C1_MF2_In2[2]);
  C1_I2[2] = FuzT(in12, C1_MF3_In2[0], C1_MF3_In2[1], C1_MF3_In2[2]);
  C1_I2[3] = FuzT(in12, C1_MF4_In2[0], C1_MF4_In2[1], C1_MF4_In2[2]);
  C1_I2[4] = FuzS(in12, C1_MF5_In2[0], C1_MF5_In2[1]);



    // Evaluación de funciones de membresía para Delta Y
  C2_I1[0] = FuzZ(in21, C2_MF1_In1[0], C2_MF1_In1[1]);
  C2_I1[1] = FuzT(in21, C2_MF2_In1[0], C2_MF2_In1[1], C2_MF2_In1[2]);
  C2_I1[2] = FuzT(in21, C2_MF3_In1[0], C2_MF3_In1[1], C2_MF3_In1[2]);
  C2_I1[3] = FuzT(in21, C2_MF4_In1[0], C2_MF4_In1[1], C2_MF4_In1[2]);
  C2_I1[4] = FuzS(in21, C2_MF5_In1[0], C2_MF5_In1[1]);

  // Evaluación de funciones de membresía para Delta Punto Y
  C2_I2[0] = FuzZ(in22, C2_MF1_In2[0], C2_MF1_In2[1]);
  C2_I2[1] = FuzT(in22, C2_MF2_In2[0], C2_MF2_In2[1], C2_MF2_In2[2]);
  C2_I2[2] = FuzT(in22, C2_MF3_In2[0], C2_MF3_In2[1], C2_MF3_In2[2]);
  C2_I2[3] = FuzT(in22, C2_MF4_In2[0], C2_MF4_In2[1], C2_MF4_In2[2]);
  C2_I2[4] = FuzS(in22, C2_MF5_In2[0], C2_MF5_In2[1]);

  // Cálculo de inferencias difusas y acumulación de salida
  int cont = 0;
  int o1,o2;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      C1_I[cont] = C1_I1[i] * C1_I2[j]; //C1_I=C1_I1'*C1_I2;
      C2_I[cont] = C2_I1[i] * C2_I2[j]; //C1_I=C1_I1'*C1_I2;
      out1 = C1_I[cont] * C1_Out[cont] + out1;
      out2 = C2_I[cont] * C2_Out[cont] + out2;
      o1 = out1*25.5;
      o2 = out2*25.5;
      cont++;
    }
  }
  
duty_cycle = 0;
ain11 = abs(in11);
ain21 = abs(in21);

if (ain11 > ain21) {
    if (in11 > 05.0) {
        duty_cycle = o1;
        cmd = 2;
    } 
    if (in11 < -05.0) {
        duty_cycle = abs(o1);
        cmd = 3;
    }
    // if (in11 >= -1.0 || in11 <= 1.0) {
    //     duty_cycle = 0;
    //     cmd = 0;
    // }
} 
else if (ain11 < ain21) {
    if (in21 >50.0) {
        duty_cycle = o2;
        cmd = 1;
    } 
    if (in21 < -50.0) {
        duty_cycle = abs(o2);
        cmd = -1;
    } 
    // if (in21 >= -1.0 || in21 <= 1.0) {
    //     duty_cycle = 0;
    //     cmd = 0;
    // }
} 
else if (ain11 <= 05 && ain21 <= 50) {
    duty_cycle = 0;
    cmd = 0;
}




  Serial.print(o1);
  Serial.print(" ");
  Serial.print(o2);
  Serial.print(" ");
  Serial.print(in11);
  Serial.print(" ");
  Serial.print(in21);
  Serial.print(" ");
  Serial.println(cmd);

//    analogWrite(m1, o1);
//    analogWrite(m2, o2)
      f_update_motors_cmd(duty_cycle, &cmd);

    
  delay(100);
}

float FuzT(float u, float a, float b, float c) {
  if (u <= a) return 0.0;
  if (u > a && u <= b) return (u - a) / (b - a);
  if (u > b && u < c) return (c - u) / (c - b);
  if (u >= c)  return 0.0;
}

float FuzZ(float u, float a, float b) {
  if (u <= a) return 1.0;
  if (u > a && u < b) return (u - a) / (b - a);
  if (u >= b) return 0.0;
}

float FuzS(float u, float a, float b) {
  if (u <= a) return 0.0;
  if (u > a && u < b) return (u - a) / (b - a);
  if (u >= b) return 1.0;
  }

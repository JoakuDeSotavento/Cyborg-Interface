#include <CapacitiveSensor.h>


// 10M resistor between pins 4 & 2, pin 2 is sensor pin
// La resietencia varia segun el material que se use como sensor
// Es este caso la mejor resistencia esta entre 2 y 3 M, 4.5 M es demasiado
// El primer argumento es el sensor y el segundo es el pin que manda HIGH
// Este programa esta pensado para un Arduino Mega, por la cantidad de pines digitales

//sensor 1
CapacitiveSensor   cs_12_11 = CapacitiveSensor(12, 11);
CapacitiveSensor   cs_10_9 = CapacitiveSensor(10, 9);

int retard = 200;

//const int BUZZER = 13;
//const int LED = 7;

//int umbral = 11000;

const int ON_12_11 = 1000;
const int ON_10_9 = 1000;

//int numBot = 1;

int bot_12_11 = 0;
int bot_10_9 = 0;


void setup()
{
  // turn off autocalibrate on channel 1 - just as an example

  cs_12_11.set_CS_AutocaL_Millis(0xFFFFFFFF);
  cs_10_9.set_CS_AutocaL_Millis(0xFFFFFFFF);

  Serial.begin(9600);
  //pinMode(BUZZER, OUTPUT);
  //pinMode(LED, OUTPUT);

}

void loop()
{
  long start = millis();

  // se asigna una sensibilidad al sensor.
  // Es aquí y en el valor de la resistencia que se controla la sensibilidad de los sensores

  long total_12_11  =  cs_12_11.capacitiveSensor(12);
  long total_10_9  =  cs_10_9.capacitiveSensor(12);

  // estas lineas sirven para probar la sensibilidad de los sensores cuando se cambia de material
  // con hierro funciona bien 1M, con cobre funciona bien 2M, 3M y hasta 4M. 5M y 10M es demasiado.
  /*
    Serial.print("S_12_11_");
   Serial.print(total_12_11);
   Serial.print("\t");
   Serial.print("S_10_9_");
   Serial.print(total_10_9);
   Serial.print("\t");
   Serial.print("S_8_7_");
   Serial.print(total_8_7);
   Serial.print("\t");
   Serial.print("S_6_5_");
   Serial.print(total_6_5);
   Serial.print("\t");
   Serial.print("S_4_3_");
   Serial.print(total_4_3);
   Serial.print("\t");
   Serial.print("S_22_24_");
   Serial.print(total_22_24);
   Serial.print("\t");
   Serial.print("S_26_28_");
   Serial.print(total_26_28);
   Serial.print("\t");
   Serial.print("S_30_32_");
   Serial.print(total_30_32);
   Serial.print("\t");
   Serial.print("S_34_36_");
   Serial.print(total_34_36);
   Serial.print("\t");
   Serial.print("S_38_40_");
   Serial.print(total_38_40);
   Serial.print("\t");
   Serial.print("S_42_44_");
   Serial.print(total_42_44);
   Serial.print("\t");
   Serial.print("S_46_48_");
   Serial.print(total_46_48);
   Serial.println("\t");   
   */

  if (total_12_11 > ON_12_11)
  {
    bot_12_11 = 1;
  }
  if (total_10_9 > ON_10_9) {
    bot_10_9 = 1;
  }

  Serial.print(bot_12_11);
  Serial.print(',');
  Serial.println(bot_10_9);

  //  for (int b = 0; b < numBot: b++ ){
  //
  //    
  //    
  //  }

  bot_12_11 = 0;
  bot_10_9 = 0;

  delay(retard);                             // arbitrary delay to limit data to serial port
}






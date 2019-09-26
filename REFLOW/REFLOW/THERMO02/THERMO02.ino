#include "max6675.h"

int thermoDO = A2;
int thermoCS = A1;
int thermoCLK = A0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

  
void setup() {
  Serial.begin(9600);
  
  Serial.println("MAX6675 test");
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  
   Serial.print("C = "); 
   Serial.println(thermocouple.readCelsius());
 
   delay(1000);
}

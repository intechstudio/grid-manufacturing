void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT); //  /OE
  pinMode(1, OUTPUT); //   LE
  pinMode(2, OUTPUT); //   DATA


//RESET

  
}

void loop() {

  digitalWrite(0, LOW);
  digitalWrite(1, HIGH);
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  

  delay(1000);

  
  digitalWrite(0, LOW);
  digitalWrite(1, HIGH);
  digitalWrite(1, LOW);
  digitalWrite(2, HIGH);

  delay(1000);

  
  // put your main code here, to run repeatedly:

}

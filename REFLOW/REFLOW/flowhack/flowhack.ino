void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(4, LOW); // Common cathode
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:



digitalWrite(7, HIGH);
shiftOut(2,3,LSBFIRST,B11110010);
digitalWrite(7, LOW);


delay(100);



}

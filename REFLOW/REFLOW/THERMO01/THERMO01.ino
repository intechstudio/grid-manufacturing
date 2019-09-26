/*
 Temperature Reading from a MAX6675
 
 Ryan McLaughlin <ryanjmclaughlin@gmail.com>
*/

#define SO 12    // MISO
#define SCK 13   // Serial Clock
#define TC_0 11  // CS Pin of MAX6607

int TC_0_calib = 0;  // Calibration compensation value in digital counts (.25[ch730]C)

void setup() {

pinMode(SO, INPUT);
pinMode(SCK, OUTPUT);

pinMode(TC_0, OUTPUT);
digitalWrite(TC_0,HIGH);  // Disable device

Serial.begin(9600);
}


/* Create a function read_temp that returns an unsigned int
  with the temp from the specified pin (if multiple MAX6675).  The
  function will return 9999 if the TC is open.
 
  Usage: read_temp(int pin, int type, int error)
    pin: the CS pin of the MAX6675
    type: 0 for [ch730]F, 1 for [ch730]C
    error: error compensation in digital counts
    samples: number of measurement samples (max:10)
*/
unsigned int read_temp(int pin, int type, int error, int samples) {
 unsigned int value = 0;
 int error_tc;
 float temp;
 unsigned int temp_out;
 
 for (int i=samples; i>0; i--){
   digitalWrite(pin,LOW); // Enable device

   /* Cycle the clock for dummy bit 15 */
   digitalWrite(SCK,HIGH);
   digitalWrite(SCK,LOW);

   /* Read bits 14-3 from MAX6675 for the Temp
      Loop for each bit reading the value and
      storing the final value in 'temp'
   */
   for (int i=11; i>=0; i--){
     digitalWrite(SCK,HIGH);  // Set Clock to HIGH
     value += digitalRead(SO) << i;  // Read data and add it to our variable
     digitalWrite(SCK,LOW);  // Set Clock to LOW
   }
 
   /* Read the TC Input inp to check for TC Errors */
   digitalWrite(SCK,HIGH); // Set Clock to HIGH
   error_tc = digitalRead(SO); // Read data
   digitalWrite(SCK,LOW);  // Set Clock to LOW
 
   digitalWrite(pin, HIGH); //Disable Device
 }
 
 value = value/samples;  // Divide the value by the number of samples to get the average
 
 /*
    Keep in mind that the temp that was just read is on the digital scale
    from 0[ch730]C to 1023.75[ch730]C at a resolution of 2^12.  We now need to convert
    to an actual readable temperature (this drove me nuts until I figured
    this out!).  Now multiply by 0.25.  I tried to avoid float math but
    it is tough to do a good conversion to [ch730]F.  THe final value is converted
    to an int and returned at x10 power.
   
  */
 
 value = value + error;  // Insert the calibration error value
 
 if(type == 0) {  // Request temp in [ch730]F
   temp = ((value*0.25) * (9.0/5.0)) + 32.0;  // Convert value to [ch730]F (ensure proper floats!)
 } else if(type == 1) {  // Request temp in [ch730]C
   temp = (value*0.25);  // Multiply the value by 25 to get temp in [ch730]C
 }
 
 temp_out = temp*10;  // Send the float to an int (X10) for ease of printing.
 
 /* Output 9999 if there is a TC error, otherwise return 'temp' */
 if(error_tc != 0) { return 9999; } else { return temp_out; }
}

void loop() {
 
 // Read the temperature and print it to serial
 Serial.print("Temp F: ");
 Serial.print(read_temp(TC_0,0,TC_0_calib,10));  
 Serial.print("\tTemp C: ");
 Serial.println(read_temp(TC_0,1,TC_0_calib,10));

 delay(250);
}

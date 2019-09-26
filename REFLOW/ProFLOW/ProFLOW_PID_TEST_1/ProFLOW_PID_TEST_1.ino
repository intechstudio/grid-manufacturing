#define ZERO    B11111100
#define ONE     B01100000
#define TWO     B11011010
#define THREE   B11110010
#define FOUR    B01100110
#define FIVE    B10110110
#define SIX     B10111110
#define SEVEN   B11100000
#define EIGHT   B11111110
#define NINE    B11110110

#define A       B11101110
#define B       B00111110
#define C       B00011010
#define D       B01111010
#define E       B10011110
#define F       B10001110

#define G       B10111100
#define H       B00101110
#define I       B01100000
#define J       B01111000
#define K       B01101110
#define L       B00011100
#define M       B10101000
#define N       B00101010

#define O       B00111010
#define P       B11001110
#define Q       B11100110
#define R       B00001010
#define S       B10110110
#define T       B00011110
#define U       B01111100
#define V       B00111000

#define W       B01010100
#define X       B01111110
#define Y       B01001110
#define Z       B11011010

#define SPACE   B00000000
#define MINUS   B00000010

#define ON      HIGH
#define OFF     LOW

const byte number  [10]=  {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

const int profile1[220]={

   20,  22,  24,  26,  28,  30,  32,  34,  36,  38,
   40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
   
   50,  51,  53,  54,  56,  57,  59,  60,  62,  63,
   65,  66,  68,  69,  71,  72,  74,  75,  77,  78,
   80,  81,  83,  84,  86,  87,  89,  90,  92,  93,
   95,  96,  98,  99, 101, 102, 104, 105, 107, 108,
  110, 111, 113, 114, 116, 117, 119, 120, 122, 123,
  125, 126, 128, 129, 131, 132, 134, 135, 137, 138,
  140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
  150, 150, 151, 151, 152, 152, 153, 153, 154, 154,
  155, 155, 156, 156, 157, 157, 158, 158, 159, 159,
  160, 160, 161, 161, 162, 162, 163, 163, 164, 164,
  165, 165, 166, 166, 167, 167, 168, 168, 169, 169,
  170, 170, 171, 171, 172, 172, 173, 173, 174, 174,
  175, 175, 176, 176, 177, 177, 178, 178, 179, 179,
  180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
  190, 191, 193, 194, 196, 197, 199, 200, 202, 203,
  205, 206, 208, 209, 211, 212, 214, 215, 217, 218,
  220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
  230, 229, 228, 227, 226, 225, 224, 223, 222, 221,
  220, 218, 216, 214, 212, 210, 208, 206, 204, 200,
  180, 160, 140, 120, 100,  80,  60,  40,  30,  20,
  
  
  };

const int profile_2[44]={


   25,  26,  27,  28,  29,  30,  31,  32,  33,  36, 
   40,  54,  78,  98, 114, 127, 136, 142, 147, 150,
  151, 152, 153, 154, 155, 156, 160, 166, 175, 186,
  200, 217, 235, 249, 253, 253, 249, 235, 217, 194, 
  171, 150, 129, 112
  
};



#define MAX6675_CS   A1
#define MAX6675_SO   A2
#define MAX6675_SCK  A0

double readThermocouple() {

  uint16_t v;
  pinMode(MAX6675_CS, OUTPUT);
  pinMode(MAX6675_SO, INPUT);
  pinMode(MAX6675_SCK, OUTPUT);
  
  digitalWrite(MAX6675_CS, LOW);
  delay(1);

  // Read in 16 bits,
  //  15    = 0 always
  //  14..2 = 0.25 degree counts MSB First
  //  2     = 1 if thermocouple is open circuit  
  //  1..0  = uninteresting status
  
  v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  v <<= 8;
  v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  
  digitalWrite(MAX6675_CS, HIGH);
  if (v & 0x4) 
  {    
    // Bit 2 indicates if the thermocouple is disconnected
    return NAN;     
  }

  // The lower three bits (0,1,2) are discarded status bits
  v >>= 3;

  // The remaining bits are the number of 0.25 degree (C) counts

  //Serial.println(v*0.25);
  
  return v*0.25;
}

double readThermocoupleOvesample(int num) {

  double temp = 0;

  for(int i=0; i<num; i++){
    
    temp+=readThermocouple();
  }
  
  return temp/num;
}


void displayRefresh(byte a, byte b, byte c, byte d, long wait=0){

byte disp[4]={d, c, b, a};
byte active=0;
byte i=0;

while(wait>0){

// ACTIVATE SELECTED DIGIT
digitalWrite(2, HIGH);
digitalWrite(3, HIGH);
digitalWrite(4, HIGH);
digitalWrite(5, HIGH);

// SEND SEGMENT DATA
//digitalWrite(13,  disp[active]&B00000001); // a
digitalWrite(12,  disp[active]&B00000010); // b
digitalWrite(11,  disp[active]&B00000100); // c
digitalWrite(10,  disp[active]&B00001000); // d
digitalWrite(9 , disp[active]&B00010000); // e
digitalWrite(8 , disp[active]&B00100000); // f
digitalWrite(7 , disp[active]&B01000000); // g
digitalWrite(6 , disp[active]&B10000000); // dp
digitalWrite(active+2, LOW);
delay(1);
active++;
active=active%4;
wait--;  
}

digitalWrite(2, HIGH);
digitalWrite(3, HIGH);
digitalWrite(4, HIGH);
digitalWrite(5, HIGH);
}

void setup() {
  
  delay(500);

  Serial.begin(115200);

  // DIGIT SELECT
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  // SEGMENT FILL
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  // HEATING ELEMENT (RED LED)
  pinMode(13, OUTPUT);
  // ROTARY ENCODER
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);



/* TEST INTERPOLATION
  
  for(int i=0; i<430-1; i++){

     Serial.print(profile_2[i/10]);
     Serial.print(" ");
     //Serial.println(profile_2[i/10]+((profile_2[i/10+1]-profile_2[i/10])*(i%10))/10);
     Serial.println(profile_interpolation(profile_2, i));

  }
  while(1){}

 */

}

// Offset for calculating cooking time
unsigned long starttime=0;
int temp=0;

byte state=0;
byte profile=0;

void heat(int on) {

  digitalWrite(13, on);
 
}

void stamp(){
  
  Serial.print((millis()-starttime)/1000);
  Serial.print("  ");
  Serial.print(temp);
  Serial.println("°C");
  
  
}

float profile_interpolation(int* profile, int index){
  
  return profile[index/10]+((profile[index/10+1]-profile[index/10])*(index%10))/10.0;
  
}

double target = 0;

double error       = 0;
double error_last  = 0;
int loopcounter = 0;

double difference = 0;

int PID_P = 0;
int PID_I = 0;
int PID_D = 0;
int PID_X = 0;

      
#define Ap 350
#define Pd 2000
#define Ax 200 // 300 volt

void loop() {

// DEFAULT STATE
  if (state==0){
    
    
    heat(OFF); 

    double temperature = 0;
    int value = 0;
    long int pwm = 0; //0...1000
    //while(!digitalRead(A3));
    
    temperature = readThermocoupleOvesample(100);
    value = (int)temperature; // int for led diplay


    // FUTURE MAGIC VARIABLE
    PID_X = (profile_interpolation(profile_2, loopcounter+10) - profile_interpolation(profile_2, loopcounter))*Ax;

    if (PID_X<0){
      PID_X=0;  
    }
    
    error_last = error;
    error = target - temperature;

    difference = error - error_last;
   
      
    PID_P = error*Ap;
    PID_I = 0;
    PID_D = difference*Pd;

    pwm = PID_P + PID_I + PID_D + PID_X;


    if (error<0){
      //pwm = 0;  
    }

    // LIMIT PWM RANGE
    (pwm>1000)?pwm=1000:1;
    (pwm<0   )?pwm=0   :1;

    if (loopcounter>2){
      
      Serial.print(target);
      Serial.print(" "); 
      Serial.print(temperature);
      Serial.print(" "); 
      Serial.print(PID_P/20.0);
      Serial.print(" "); 
      Serial.print(PID_D/20.0);
      Serial.print(" "); 
      Serial.print(error);
      Serial.print(" "); 
      Serial.print(pwm/10.0); //in duty cycle percentage
      Serial.print(" "); 
      Serial.println(PID_X/20.0); //in duty cycle percentage
      
    }


    if(pwm>0){
      heat(ON);
    }
    
    displayRefresh(number[value/100%10],number[value/10%10],number[value%10],C,pwm);

    heat(OFF);
    displayRefresh(number[value/100%10],number[value/10%10],number[value%10],C,1000-pwm);


    if (loopcounter<430-1){

      //target = profile1[loopcounter/2];
      target = profile_interpolation(profile_2, loopcounter);
      loopcounter++;
    }
    else{
      target = 20;  
    }

    

/*
    if (loopcounter==200){
      if (target==50){
        target=100;  
      }  
      else{
        target=50;
      }

      loopcounter=0;
    }
    else{
      loopcounter++; 
    }
*/
    
    //if (!digitalRead(A3)){state++;}
  }


  

// SELECT PRESET
  
  if (state==1){  
    while(!digitalRead(A3));
    byte encA=0;
    byte encB=0;
    byte dire=0;
    int encV=0;
    while(state==1){
      if (!digitalRead(A3)){
        state=2; // Start Preheat
        profile=encV%10;
        //Serial.print("Staring Profile: TP-");
        //Serial.println(profile);
        starttime = millis();
      
      } // BUTTONPRESS

      if (digitalRead(A4) == 1 && digitalRead(A5) ==1){ // Stable
        if (!encB &&  dire){encV++;}
        if (!encA && !dire){encV--;}  
      }
      else if (encB && encA){
        dire=digitalRead(A5);        
      }

      encV=(encV>9)?9:encV;
      encV=(encV<0)?0:encV;
      
      encA=digitalRead(A4);
      encB=digitalRead(A5);

      displayRefresh(T,P,MINUS,number[encV%10],4);
      
    }
  }

// PRE HEAT // T < 120°C // (90sec)

  if (state==2){
    while(!digitalRead(A3));
    heat(ON);
    while (state==2){
      displayRefresh(SPACE,P,R,E,1000);
      temp=readThermocouple();
      stamp();
      displayRefresh(number[temp/100%10],number[temp/10%10],number[temp%10],C,1000);
      temp=readThermocouple();
      stamp();    
      // PREHEAT DONE!
      if (temp>120){
        //Serial.println("Soak Zone");
        state=3;
        heat(OFF);    
      }
      if (!digitalRead(A3)){
        //Serial.println("Abort");
        state=0;  
        heat(OFF); 
      }
    }
  }

// SOAKING // 120°C < T < 180°C // (90sec)

  if (state==3){
    while(!digitalRead(A3));
    while (state==3){
      heat(ON); 
      displayRefresh(S,O,A,K,1000);
      temp=readThermocouple();
      stamp();

      heat(OFF); 
      displayRefresh(number[temp/100%10],number[temp/10%10],number[temp%10],C,1000);
      temp=readThermocouple();
      stamp();   
       
      // PREHEAT DONE!
      if (temp>180){
        //Serial.println("Reflow Zone");
        state=4;
        heat(OFF);    
      }
      if (!digitalRead(A3)){
        //Serial.println("Abort");
        state=0;  
        heat(OFF); 
      }
    }
  }

// FLOW // 180°C < T < 230°C // (30sec)

  if (state==4){
    while(!digitalRead(A3));
    while (state==4){
      heat(ON); 
      displayRefresh(F,L,O,W,1000);
      temp=readThermocouple();
      stamp();

      heat(ON); 
      displayRefresh(number[temp/100%10],number[temp/10%10],number[temp%10],C,1000);
      temp=readThermocouple();
      stamp();   
       
      if (temp>220){
        //Serial.println("Cool");
        state=5;
        heat(OFF);    
      }
      if (!digitalRead(A3)){
        //Serial.println("Abort");
        state=0;  
        heat(OFF); 
      }
    }
  }

// COOL // T < 230°C
  if (state==5){
    while(!digitalRead(A3));
    while (state==5){
      heat(OFF); 
      displayRefresh(C,O,O,L,1000);
      temp=readThermocouple();
      stamp();

      heat(OFF); 
      displayRefresh(number[temp/100%10],number[temp/10%10],number[temp%10],C,1000);
      temp=readThermocouple();
      stamp();   
       
      if (temp<200){
        //Serial.println("Done");
        state=0;
        heat(OFF);    
      }
      if (!digitalRead(A3)){
        //Serial.println("Abort");
        state=0;  
        heat(OFF); 
      }
    }
  }

  // DONE
  if (state==6){  
    while(!digitalRead(A3));
    byte b=0;
    while(b<200){
    displayRefresh(D,O,N,E,4);
    if (!digitalRead(A3)){state=0;}
    b++;}
    b=0;
    while(b<200){
    displayRefresh(SPACE,SPACE,SPACE,SPACE,4);
    if (!digitalRead(A3)){state=0;}
    b++;}
  }


}

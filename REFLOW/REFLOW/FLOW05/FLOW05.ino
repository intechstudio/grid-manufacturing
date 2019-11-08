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


const byte number  [37]=  {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, A, B, C, D, E, F,G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, SPACE};


//                   t = 0,  10, 20, 30,  40,  50,  60,  70,

const byte profile1[190]={

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
  220, 218, 216, 214, 212, 210, 208, 206, 204, 202
 
  
  };

#include "max6675.h"





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
digitalWrite(13,  disp[active]&B00000001); // a
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


MAX6675 thermocouple(A0, A1, A2); // DO CS CLK

void setup() {

  delay(500);

  Serial.begin(9600);


  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);

  

}

int value=0;
int target=0;

unsigned long timeoffset=0;
unsigned long timecurrent=0;
int process=0;

byte state=0;

int getC(int sample=5){

  double raw = 0;
  
  for (int i=0; i<sample; i++){
    raw += thermocouple.readCelsius();
  }

  return raw/sample;

}

void loop() {


// ENCODER TEST
  if (state==20){
    byte encA=0;
    byte encB=0;
    byte dire=0;
    byte encV=0;
    while(state==20){
      if (!digitalRead(A3)){state=0;} // BUTTONPRESS

      if (digitalRead(A4) == 1 && digitalRead(A5) ==1){ // Stable
        if (!encB &&  dire){encV++;}
        if (!encA && !dire){encV--;}  
      }
      else if (encB && encA){
        dire=digitalRead(A5);        
      }
      
      encA=digitalRead(A4);
      encB=digitalRead(A5);

      displayRefresh(number[encV/100%10],number[encV/10%10],number[encV%10],E,4);
    
    }
  }


// DEFAULT STATE
  if (state==0){
    while(!digitalRead(A3));

    value=getC(5);
    displayRefresh(number[value/100%10],number[value/10%10],number[value%10],C,200);
    if (!digitalRead(A3)){state++;}
  }

// SELECT PRESET
  
  if (state==1){  
    while(!digitalRead(A3));
    byte encA=0;
    byte encB=0;
    byte dire=0;
    int encV=0;
    while(state==1){
      if (!digitalRead(A3)){state=2;} // BUTTONPRESS

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

// PRE HEAT // T < 140°C // (90sec)

  if (state==2){
    while(!digitalRead(A3));
    displayRefresh(SPACE,P,R,E,1000);
    delay(1000);
    state=3;
  }

// SOAKING // 140°C < T < 180°C // (90sec)

  if (state==3){
    while(!digitalRead(A3));
    displayRefresh(S,O,A,K,1000);
    delay(1000);
    state=4;
  }

// FLOW // 180°C < T < 230°C // (30sec)

  if (state==4){
    while(!digitalRead(A3));
    displayRefresh(F,L,O,W,1000);
    delay(1000);
    state=5;
  }

// COOL // T < 230°C
  if (state==5){
    while(!digitalRead(A3));
    displayRefresh(C,O,O,L,1000);
    delay(1000);
    state=6;
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

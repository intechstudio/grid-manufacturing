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
#define C       B10011100
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

MAX6675 thermocouple(14, 16, 10); // DO CS CLK
int vccPin = 9;
int gndPin = 8;


void displayRefresh(byte a, byte b, byte c, byte d, long wait=0, byte type=0){

if (type==0){
a=number[a];
b=number[b];
c=number[c];
d=number[d];
}

while(wait>0){

    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    shiftOut(2,3,LSBFIRST,d);
    digitalWrite(4, LOW);
    delay(1);
    digitalWrite(4, HIGH);
    shiftOut(2,3,LSBFIRST,c);
    digitalWrite(5, LOW);
    delay(1);
    digitalWrite(5, HIGH);
    shiftOut(2,3,LSBFIRST,b);
    digitalWrite(6, LOW);
    delay(1);
    digitalWrite(6, HIGH);
    shiftOut(2,3,LSBFIRST,a);
    digitalWrite(7, LOW);
    delay(1);

wait--;  
}

}


void setup() {

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  
  Serial.begin(9600);
  pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  Serial.println("MAX6675 test");
  delay(500); 
}

double value=0;
int target=0;
int value2=0;

unsigned long timeoffset=0;
unsigned long timecurrent=0;
int process=0;

void loop() {

  while(Serial.available()){
    char incoming = Serial.read();
    if (incoming=='s' || incoming=='S'){
      process=1;  
      Serial.println("Start!");
      timeoffset=floor(millis()/1000)*1000;
    }
    else{
      process=0; 
      Serial.println("Stop!"); 
    }
  
  }

  timecurrent=millis()-timeoffset;
  value= thermocouple.readCelsius();

  if ((timecurrent/1000)>188 && (process!=0 || process!=3)){
    // ERROR
    digitalWrite(A1, LOW);
    Serial.println("188-DONE!"); 
    process=4;
  }

  if (process==2){
    target=profile1[timecurrent/1000];
  }


  if (process==0){
    digitalWrite(A1, LOW);   
  }
  
  // PREHEATING ELEMENTS
  if (process==1){
    if (value<48){
      digitalWrite(A1, HIGH);
    }
    else{
      timeoffset=floor(millis()/1000)*1000;
      process=2; 
      Serial.println("Process 2!"); 
    } 
  }
  if (process==2){
    if (value<target){
      digitalWrite(A1, HIGH);     
    }
    else{
      digitalWrite(A1, LOW);  
    }

    if (value>240){
      process=3;
      Serial.println("Process 3!"); 
      digitalWrite(A1, LOW); 
      Serial.println("240C Overheat!");     
    }
  }

  while ((timecurrent+timeoffset)/1000%10==millis()/1000%10){
    displayRefresh(number[process],SPACE,SPACE,SPACE,1,1);
  }

  Serial.print("  t = "); 
  Serial.print(timecurrent/1000);  
    
  Serial.print("    C = "); 
  Serial.print(value);
  
  Serial.print("    Target = "); 
  Serial.print(target);
  Serial.print("    delta = "); 
  Serial.print((value-target));

  Serial.print("    p = "); 
  Serial.println(process);

}

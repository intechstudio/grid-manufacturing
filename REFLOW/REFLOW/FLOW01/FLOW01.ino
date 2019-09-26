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


const byte number  [36]=  {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, A, B, C, D, E, F,G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z};


void displayRefresh(byte a, byte b, byte c, byte d, long wait=0){

a=number[a];
b=number[b];
c=number[c];
d=number[d];


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

  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH); 
  digitalWrite(2, LOW);

 
}

void loop() {

displayRefresh(millis()/100000%10,millis()/10000%10,millis()/1000%10,millis()/100%10,1);

}

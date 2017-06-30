int rightTurn=0;
int leftTurn=0;
int frontTurn=0;
int backTurn=0;
int bottomTurn=0;
int topTurn=0;

/*
 * 
 * 
 * 2x2 NeoPixel Rubik's Cube code. 
 * This code controls the colors of the lights. There are 6 switches that control the movements of the cube.
 * by Matthew Bardoe July 21, 2016
 * 
 * The arrangement of the pixels is (everything one bigger than it should be)
 *  ??? WHERE IS 9????
 *              22  23
 *              21  24
 *              ------
 *              20  14
                15  9
                ------
      19  16 |  7   8 | 10  13
      18  17 |  6   5 | 11  12
                ------
                3   4
                2   1
 * /
 */// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            11

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24
#define SWITCH 0
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// an array to keep track of the colors.
char cube[] ={'r', 'r', 'r', 'r', 'w', 'w', 'w', 'w', 'o',  'y', 'y', 'y', 'y', 'o','o', 'g', 'g', 'g', 'g', 'o', 'b', 'b', 'b', 'b'};
//char cube[] ={'o', 'o', 'y', 'y', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r','r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r', 'r','r'};

// pins for the switches
const int rightPin=3;
const int leftPin=4;
const int frontPin=5;
const int backPin=6;
const int topPin=7;
const int bottomPin=8;


void setup() {
  
  // start the pixels
  pixels.begin(); // This initializes the NeoPixel library.
  // start the pins.
  pinMode(rightPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(frontPin, INPUT);
  pinMode(backPin, INPUT);
  pinMode(topPin, INPUT);
  pinMode(bottomPin, INPUT);
  Serial.begin(9600);
  
}

void loop() {
  
  // read the buttons
  //int rightTurn=digitalRead(rightPin);
  int leftTurn=digitalRead(leftPin);
  int frontTurn=digitalRead(frontPin);
  
  //int backTurn=digitalRead(backPin);
  //int bottomTurn=digitalRead(bottomPin);
  int topTurn=digitalRead(topPin);
  
  Serial.print("front: ");
  Serial.println(frontTurn);
  
  Serial.print("left: ");
  Serial.println(leftTurn);
  
  Serial.print("top: ");
  Serial.println(topTurn);
  Serial.println(cube);
  // determine if a button pressed
  if (rightTurn==HIGH){
    twistRight();   
  }
  else if (leftTurn==HIGH){
    twistLeft();
  }
  else if (frontTurn==HIGH){
    twistFront();
  }
  else if (backTurn==HIGH){
    twistBack();
  }
  else if (bottomTurn==HIGH){
    twistBottom();
  }
  else if (topTurn==HIGH){
    twistTop();
  }
  // Show the lights
  displayCube();
  // debounce
  delay(300);
}

// code to twist a cube on the left all twist codes work the same way. It shuffles the values of cube, and then string copies it 
// back to cube. (arduino is tough with arrays).
void twistLeft(){
   delay(200);
  // this is the shuffle that completes this move.
  char shuffle[]={1, 6, 7, 4, 5, 15, 20, 8, 9, 10, 11, 12, 13, 14, 21, 19, 16, 17, 18, 22, 2, 3, 23, 24};
  // a new array to hold the shuffled cube
  char returnArray[24];
  // shuffle
  for (int i=0; i<NUMPIXELS; i++){
    // minus one here because we labelled like morons. START AT ZERO!
    returnArray[i]=cube[shuffle[i]-1];
  }
  // copy back to cube.
  strcpy(cube, returnArray);
  delay(300);
}

void twistRight(){
  char shuffle[]={5, 2, 3, 8, 9, 6, 7, 14, 24, 13, 10, 11, 12, 23, 15, 16, 17, 18, 19, 20, 21, 22, 1, 4};
  char returnArray[24];
  for (int i=0; i<NUMPIXELS; i++){
    returnArray[i]=cube[shuffle[i]-1];
  }
  strcpy(cube, returnArray);
}

void twistTop(){
  delay(200);
  char shuffle[]={1, 2, 11, 10, 8, 5, 6, 7, 16, 15, 9, 12, 13, 14, 17, 3, 4, 18, 19, 20, 21, 22, 23, 24};
  char returnArray[24];
  for (int i=0; i<NUMPIXELS; i++){
    returnArray[i]=cube[shuffle[i]-1];
  }
  strcpy(cube, returnArray);
  delay(300);
}

void twistBottom(){
  char shuffle[]={13, 12, 3, 4, 5, 6, 7, 8, 9, 10, 11, 14, 20, 19, 15, 16, 17, 1, 2, 18, 22, 23, 24, 21};
  char returnArray[24];
  for (int i=0; i<NUMPIXELS; i++){
    returnArray[i]=cube[shuffle[i]-1];
  }
  strcpy(cube, returnArray);
}

void twistFront(){
  delay(200);
  char shuffle[]={2, 3, 4, 1, 12, 11, 7, 8, 9, 10, 23, 22, 13, 14, 15, 16, 5, 6, 19, 20, 21, 17, 18, 24};
  //char shuffle[]={ 24, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 1};
  
  char returnArray[24];
  //Serial.print("cube");
  //Serial.println(cube);
  for (int i=0; i<NUMPIXELS; i++){
    returnArray[i]=cube[shuffle[i]-1];
  }
  //Serial.print("return cube:");
  //Serial.println(returnArray);
  //Serial.println(sizeof(returnArray));
  strcpy(cube, returnArray);
  //Serial.println(sizeof(cube));
  delay(400);
}

void twistBack(){
  // this is the shuffle that completes this move
  char shuffle[]={1, 2, 3, 4, 5, 6, 10, 13, 14, 24, 11, 12, 21, 20, 9, 8, 17, 18, 7, 15, 16, 22, 23, 19};
  char returnArray[24];
  for (int i=0; i<NUMPIXELS; i++){
    returnArray[i]=cube[shuffle[i]-1];
  }
  strcpy(cube, returnArray);
}

void displayCube(){
  for (int i=0;i<24;i++){
    cubeSetPixelColor(cube[i],i);
  }
  pixels.show();
}

void cubeSetPixelColor(char color, int num){
  // nice time to use a switch... If it works.
  switch (color){
    case 'r':
    pixels.setPixelColor(num, pixels.Color(0,125,0)); //fixed
    break;

    case 'o':
    pixels.setPixelColor(num, pixels.Color(0, 125 ,125));
    break;

    case 'w':
    pixels.setPixelColor(num, pixels.Color(255,255,255));
    break;

    case 'y':
    pixels.setPixelColor(num, pixels.Color(255,255,0));
    break;

    case 'g':
    pixels.setPixelColor(num, pixels.Color(125,0,0));
    break;

    case 'b':
    pixels.setPixelColor(num, pixels.Color(0,0,125));
    break;
  }
}


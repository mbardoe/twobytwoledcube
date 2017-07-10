int rightTurn=0;
int leftTurn=0;
int frontTurn=0;
int backTurn=0;
int bottomTurn=0;
int topTurn=0;
//int buttnum;
//bool pressed;

/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"
/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];


/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
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
#define PIN            10

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
  //BLE Setup
    while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit App Controller Example"));
  Serial.println(F("-----------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }


  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("******************************"));

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
  }

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("******************************"));
  
  // start the pixels
  pixels.begin(); // This initializes the NeoPixel library.
  // start the pins.
//  pinMode(rightPin, INPUT);
//  pinMode(leftPin, INPUT);
//  pinMode(frontPin, INPUT);
//  pinMode(backPin, INPUT);
//  pinMode(topPin, INPUT);
//  pinMode(bottomPin, INPUT);
//  Serial.begin(9600);
displayCube();
  
}

void loop() {
 
  /* Wait for new data to arrive */
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len == 0) return;

  /* Got a packet! */
  // printHex(packetbuffer, len);
   // Buttons
  if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
    Serial.print ("Button "); Serial.print(buttnum);
    if (pressed) {
      Serial.println(" pressed");
    } else {
      Serial.println(" released");
    }
  
    if (buttnum==1 && pressed){
//    twistRight();   
//  }
//  else if (leftTurn==HIGH){
    twistFront();
    displayCube();
  }
  }
//  else if (frontTurn==HIGH){
//    twistFront();
//  }
//  else if (backTurn==HIGH){
//    twistBack();
//  }
//  else if (bottomTurn==HIGH){
//    twistBottom();
//  }
//  else if (topTurn==HIGH){
//    twistTop();
//  }
  // Show the lights
//  displayCube();
  // debounce
//  delay(300);


  
  // read the buttons
//  //int rightTurn=digitalRead(rightPin);
//  int leftTurn=digitalRead(leftPin);
//  int frontTurn=digitalRead(frontPin);
//  
//  //int backTurn=digitalRead(backPin);
//  //int bottomTurn=digitalRead(bottomPin);
//  int topTurn=digitalRead(topPin);
//  
//  Serial.print("front: ");
//  Serial.println(frontTurn);
//  
//  Serial.print("left: ");
//  Serial.println(leftTurn);
//  
//  Serial.print("top: ");
//  Serial.println(topTurn);
//  Serial.println(cube);
  // determine if a button pressed
//  if (rightTurn==HIGH){
//    twistRight();   
//  }
//  else if (leftTurn==HIGH){
//    twistLeft();
//  }
//  else if (frontTurn==HIGH){
//    twistFront();
//  }
//  else if (backTurn==HIGH){
//    twistBack();
//  }
//  else if (bottomTurn==HIGH){
//    twistBottom();
//  }
//  else if (topTurn==HIGH){
//    twistTop();
//  }
//  // Show the lights
//  displayCube();
//  // debounce
//  delay(300);
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
  //delay(200);
  char shuffle[]={2, 3, 4, 1, 12, 11, 7, 8, 9, 10, 23, 22, 13, 14, 15, 16, 5, 6, 19, 20, 21, 17, 18, 24};
  //char shuffle[]={ 24, 2, 3, 4,  5,  6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 1};
  
  char returnArray[24];
  Serial.print("cube");
  Serial.println(cube);
  for (int i=0; i<NUMPIXELS; i++){
    returnArray[i]=cube[shuffle[i]-1];
  }
  Serial.print("return cube:");
  Serial.println(returnArray);
  Serial.println(sizeof(returnArray));
  strcpy(cube, returnArray);
  Serial.println(sizeof(cube));
  //delay(400);
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


/* 
   NRF24L01 pins:
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 9
   4 - CSN to Arduino pin 10
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
   
   Button Pins: 
   Up       - Arduino pin 2
   Right    - Arduino pin 3
   Down     - Arduino pin 4
   Left     - Arduino pin 5
   E        - Arduino pin 6
   F        - Arduino pin 7
   Joystick - Arduino pin 8
   
   Joystick pins:
   GND    to Arduino pin GND
   VCC    to Arduino pin +5V
   X Pot  to Arduino pin A0
   Y Pot  to Arduino pin A1
*/

//libraries needed:
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//Input pins needed
#define xAxis A0     
#define yAxis A1    
#define buttonUp 2
#define buttonRight 3
#define buttonDown 4
#define buttonLeft 5
#define buttonE 6
#define buttonF 7
#define buttonJoystick 8

// Radio protocole needed:
RF24 radio(9,10); // Create a Radio
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
int message[9];  // 9 messages to send 



void setup() {
  
  Serial.begin(9600); 
   
  // Radio settings
  radio.begin();
  radio.setChannel(101);  
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate( RF24_250KBPS ) ; 
  radio.openWritingPipe(pipe);
  //
  radio.stopListening();
  
  //declare pinMode for all buttons and initial state
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonRight,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);
  pinMode(buttonLeft,INPUT_PULLUP);
  pinMode(buttonJoystick,INPUT_PULLUP);
  
  digitalWrite(buttonUp,LOW);
  digitalWrite(buttonRight,LOW);
  digitalWrite(buttonDown,LOW);
  digitalWrite(buttonLeft,LOW);
  digitalWrite(buttonJoystick,LOW);

}

void loop() {

  // Define messages to send 
  message[0] = analogRead(xAxis);
  message[1] = analogRead(yAxis);
  message[2] = digitalRead(buttonUp);
  message[3] = digitalRead(buttonRight);
  message[4] = digitalRead(buttonDown);
  message[5] = digitalRead(buttonLeft);
  message[6] = digitalRead(buttonE);
  message[7] = digitalRead(buttonF);  
  message[8] = digitalRead(buttonJoystick);

  // Write messages
  radio.write( message, sizeof(message) );


  // Write what messages are sent in serial monitor for debugging
  Serial.print(" X: ");
  Serial.print(analogRead(xAxis));
  Serial.print(" Y: ");  
  Serial.print(analogRead(yAxis));
  Serial.print(" Up: "); 
  Serial.print(digitalRead(buttonUp));
  Serial.print(" Right: "); 
  Serial.print(digitalRead(buttonRight));
  Serial.print(" Down: "); 
  Serial.print(digitalRead(buttonDown));
  Serial.print(" Left: "); 
  Serial.print(digitalRead(buttonLeft));
  Serial.print(" E: "); 
  Serial.print(digitalRead(buttonE));
  Serial.print(" F: "); 
  Serial.print(digitalRead(buttonF));  
  Serial.print(" Joystick: "); 
  Serial.print(digitalRead(buttonJoystick));
  Serial.println(" ");  
}

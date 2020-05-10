#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "45623";
bool inputState = false;
//#define MY_RF24_CHANNEL 120



void setup() {
  pinMode(5, INPUT);  //Input

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}


void loop() {
  inputState = digitalRead(5);
  radio.write(&inputState, sizeof(inputState));
  delay(200);
}

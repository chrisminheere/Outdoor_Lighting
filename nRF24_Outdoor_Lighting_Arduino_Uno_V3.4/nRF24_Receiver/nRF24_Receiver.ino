#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "45623";
bool outputState = false;
//#define MY_RF24_CHANNEL 120

void setup() {
  pinMode(6, OUTPUT);  //Output PWM
  pinMode(5, OUTPUT);  //Output PWM
  pinMode(4, OUTPUT);  //Output PWM


//  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  Serial.println("OK");
  Serial.println(outputState);
}


void loop() {
  if (radio.available()) {
    digitalWrite(4, HIGH);
    digitalWrite(6, LOW);
    radio.read(&outputState, sizeof(outputState));
    digitalWrite(5, outputState);
//    Serial.println(outputState);
    delay(20);
  }
  else {
    digitalWrite(4, LOW);
    digitalWrite(6, HIGH);
  }
}

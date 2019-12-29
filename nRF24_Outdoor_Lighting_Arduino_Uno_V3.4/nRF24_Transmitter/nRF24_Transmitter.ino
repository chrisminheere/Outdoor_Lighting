#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
boolean buttonState = 0;
int inPin = 2;
int state = 0;
int k =0;

void setup() {
  Serial.begin(9600);
  pinMode(inPin, INPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MAX);
  //setDataRate( RF24_250KBPS ); 
  pinMode(13, OUTPUT);
  radio.stopListening();
}

void loop() {
  Serial.println(state);
  state = digitalRead(inPin);
  radio.write(&state, sizeof(state));
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW     
  delay(1000);
  k++;
}

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup() {
  pinMode(2, OUTPUT);

  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MAX);
  //setDataRate( RF24_250KBPS ) ; 
  radio.startListening();
}


void loop() {
  delay(10);
  if ( radio.available()) {
    while (radio.available()) {
      int state = 0;
      radio.read(&state, sizeof(state));

      if (state == 1) {
        digitalWrite(2, LOW);
      }
      else {
        digitalWrite(2, HIGH);
      }
    }
  }
}

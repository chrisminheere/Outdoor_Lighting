#include <SoftwareSerial.h>
int receivedData = 0;
int sensorPin = A0;
unsigned char dimming = 70;  // Dimming level (15-75)

SoftwareSerial mySerial(10, 11); // RX, TX


void setup()
{
  Serial.begin(9600);
  pinMode(4, OUTPUT);   // Set AC Load pin as output
  attachInterrupt(2, zero_crosss_int, RISING);  //Interupt pin

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  mySerial.begin(4800);
}




void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms
  // Every zerocrossing : (50Hz)-> 10ms (1/2 Cycle) For 60Hz (1/2 Cycle) => 8.33ms

  int dimtime = (100 * dimming);  // For 60Hz =>65
  delayMicroseconds(dimtime);     // Off cycle
  digitalWrite(12, HIGH);         // triac firing
  delayMicroseconds(10);          // triac On propogation delay (for 60Hz use 8.33)
  digitalWrite(12, LOW);          // triac Off
}


void loop() {
  Serial.println(receivedData);

if (mySerial.available() > 0) {
    Serial.write(mySerial.read());
   //receivedData = (mySerial.read());
  }

  
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

}

#include <ESP8266WiFi.h>
//#define BLYNK_PRINT Serial
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_PWMServoDriver.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 15); // RX, TX

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

char auth[] = "1a7a3e1e7e1446c7a8ce647142957ba5";
char ssid[] = "Fam-Minheere 2.4GHz";
char pass[] = "Minheere@asus";
int inPin = D2;
int val = 0;
int pinValue = 0;
int pinData = 0;
int i = 0;
int dataPin = 5;


void setup() {
  pinMode(dataPin, OUTPUT);
#ifdef ESP8266
  Wire.pins(2, 14);   // ESP8266 can use any two pins, such as SDA to #2 and SCL to #14
#endif

  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);

  
  Blynk.begin(auth, ssid, pass);
  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
}


BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  for (int i = 0; i <= 5; i++) {
    pwm.setPin(i, pinValue );//setPWM(channel, on, off)
  }
  yield();
}


BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  for (int i = 0; i <= 2; i++) {
    pwm.setPin(i, pinValue );//setPWM(channel, on, off)
  }
  yield();
}



BLYNK_WRITE(V3)
{
  int pinValue = param.asInt();
  for (int i = 3; i <= 5; i++) {
    pwm.setPin(i, pinValue );//setPWM(channel, on, off)
  }
  yield();
}


BLYNK_WRITE(V4) {

  for (int i = 0; i <= 5; i++) {
    pwm.setPin(1, 4096 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(2, 4096 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(3, 4096 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(4, 4096 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(5, 4096 );//setPWM(channel, on, off)
    delay(100);

    pwm.setPin(1, 0 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(2, 0 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(3, 0 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(4, 0 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(5, 0 );//setPWM(channel, on, off)
    delay(100);
  }


  for (int i = 0; i <= 10; i++) {
    pwm.setPin(1, 4096 );//setPWM(channel, on, off)
    pwm.setPin(2, 4096 );//setPWM(channel, on, off)
    pwm.setPin(3, 4096 );//setPWM(channel, on, off)
    pwm.setPin(4, 4096 );//setPWM(channel, on, off)
    pwm.setPin(5, 4096 );//setPWM(channel, on, off)
    delay(100);
    pwm.setPin(1, 0 );//setPWM(channel, on, off)
    pwm.setPin(2, 0 );//setPWM(channel, on, off)
    pwm.setPin(3, 0 );//setPWM(channel, on, off)
    pwm.setPin(4, 0 );//setPWM(channel, on, off)
    pwm.setPin(5, 0 );//setPWM(channel, on, off)
    delay(100);
  }


  for (int i = 0; i <= 10; i++) {
    pwm.setPin(1, 4096 );//setPWM(channel, on, off)
    delay(50);
    pwm.setPin(1, 0 );//setPWM(channel, on, off)
    delay(50);

    pwm.setPin(4, 4096 );//setPWM(channel, on, off)
    delay(50);
    pwm.setPin(4, 0 );//setPWM(channel, on, off)
    delay(50);

    pwm.setPin(2, 4096 );//setPWM(channel, on, off)
    delay(50);
    pwm.setPin(2, 0 );//setPWM(channel, on, off)
    delay(50);

    pwm.setPin(5, 4096 );//setPWM(channel, on, off)
    delay(50);
    pwm.setPin(5, 0 );//setPWM(channel, on, off)
    delay(50);

    pwm.setPin(3, 4096 );//setPWM(channel, on, off)
    delay(50);
    pwm.setPin(3, 0 );//setPWM(channel, on, off)
    delay(50);
  }
}

BLYNK_WRITE(V5) {
  pinData = param.asInt();
}

BLYNK_WRITE(V6) {
  i = param.asInt();
  mySerial.println(i);
}


void loop() {
  Blynk.run();
  val = digitalRead(inPin);
 
  //mySerial.println(i);
  Serial.println(i);
  if (pinData == 1 ) {
    if (val == LOW) {
      for (int i = 0; i <= 5; i++) {
        pwm.setPin(i, 4096 );//setPWM(channel, on, off)
      }
    }
    else {
      for (int i = 0; i <= 5; i++) {
        pwm.setPin(i, 0 );//setPWM(channel, on, off)
      }
    }
  }
  else {
  }
  delay(100);
}

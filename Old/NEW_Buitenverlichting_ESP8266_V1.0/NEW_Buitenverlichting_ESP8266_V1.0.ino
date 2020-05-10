//_______________include libraries here._______________
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_PWMServoDriver.h>
//#include <SoftwareSerial.h>
#include <BH1750.h>
#include <SensorUtil.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
//_____________________________________________________




//________________define stuff here.___________________
#define lightSensorAdress 0x23     // I2C addres of Light sensor.                        Default =  0x23
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // I2C addres of PWM breakout.  Default =  0x40
#define BLYNK_PRINT Serial       // Debugging for blynk, turn of when not needed.
//SoftwareSerial mySerial(13, 15);   // RX, TX  :for conecting to the Arduino Nano.
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 7200;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);
BH1750 lightMeter;
//const int interruptPin = 14;
//#define ICACHE_RAM_ATTR

char auth[] = "1a7a3e1e7e1446c7a8ce647142957ba5";  //Auth key
char ssid[] = "Fam-Minheere 2.4GHz";               //WIFI SSID it connects with.
char pass[] = "Minheere@asus";                     //WIFI password.
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//____________________________________________________





//____________________All variables____________________

//int inPin = D2;
int val = 0;
int pinValue = 0;
int pinData = 0;
int i = 0;
int lux = 0;
int autoMode = 1;
int currentHour = 0;
int currentMinutes = 0;

int ledPin = D6; 
int ledPin2 = D7;

//______________________SETTINGS_______________________
//Min Lux, is when the light go ON.
int minLux = 100;
//Max Lux, is when the light go OFF.
int maxLux = 150;

//Defining start day time:
int startHour = 6;
//Defining end day time:
int endHour = 2;
//_____________________________________________________




void setup() {
  //pinMode(interruptPin, INPUT);
  //attachInterrupt(13, handleInterrupt, CHANGE ); //Interupt pin 
  Serial.begin(115200);
//  mySerial.begin(4800);

  while (!Serial) {}; // wait for serial port to connect. Needed for native USB port only

  Blynk.begin(auth, ssid, pass);

  Wire.begin(4, 5);   //Define the i2c pins on the ESP8266  SDA, GPIO4 = D2     SCL, GPIO5 = D1
  lightMeter.begin(BH1750::ONE_TIME_LOW_RES_MODE);  //Begins the sensor en sets it to the LowRes Mode
  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
  timeClient.begin();
  
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

 
}


//(hours == 16 || hours == 3 || hours == 4 || hours == 5 || hours == 6 ) {  //light are OFF from 02:00 til 06:59.


void loop() {
  Blynk.run();
  

  getTime();
  lux = lightMeter.readLightLevel();
  
  Serial.print("lux ");
  Serial.println(lux);

  if (autoMode == 1 ) {
    digitalWrite(ledPin2, HIGH);
    if (lux < minLux ) {
      if (currentHour >= startHour && currentHour < endHour ) {
        turnAllOff();
      }
      else {
        turnAllOn();
      }
    }
    else if (lux > maxLux) {
      turnAllOff();
    }
  }
  else{  
     digitalWrite(ledPin2, LOW);
    }
     digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
}



//void ICACHE_RAM_ATTR handleInterrupt() { 
//    Serial.println("Interrupt Detected"); 
////  if (currentHour == 17 || currentHour == 3 || currentHour == 4 || currentHour == 5 || currentHour == 6 ) {
////  
////    }
//  }




void getTime() {
  timeClient.update();
  currentHour = timeClient.getHours();
  currentMinutes = timeClient.getMinutes();

  Serial.print("Time");
  Serial.print(currentHour);
  Serial.print(":");
  Serial.print(currentMinutes);
  Serial.print("     ");
  
  
}


void turnAllOn() {
  for (int i = 0; i <= 15; i++) {
    pwm.setPin(i, 4096 );//setPWM(channel, on, off)
  }
}

void turnAllOff() {
  for (int i = 0; i <= 15; i++) {
    pwm.setPin(i, 0 );//setPWM(channel, on, off)
  }
}






BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  for (int i = 0; i <= 15; i++) {
    pwm.setPin(i, pinValue );//setPWM(channel, on, off)
  }
  yield();
}




BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  for (int i = 0; i <= 7; i++) {
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
  autoMode = param.asInt();
}

//
//BLYNK_WRITE(V6) {
//  i = param.asInt();
//  mySerial.println(i);
//}

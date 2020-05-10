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
#include <LiquidCrystal_I2C.h>
#include <dht11.h>
//#include <DHT.h>
//_____________________________________________________




//________________define stuff here.___________________
#define lightSensorAdress 0x23     // I2C addres of Light sensor.                            Default =  0x23
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // I2C addres of PWM breakout.      Default =  0x40
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address   Default =  0x27
#define BLYNK_PRINT Serial       // Debugging for blynk, turn of when not needed.
//SoftwareSerial mySerial(13, 15);   // RX, TX  :for conecting to the Arduino Nano.
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 7200;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);
BH1750 lightMeter;
//const int interruptPin = 14;
//#define ICACHE_RAM_ATTR
#define DHT11PIN D4
dht11 DHT11;


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

int lichtsOnOff = 0;
//float t = 0;
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



//Bitmap IMG
byte verticalLine[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001
};

byte verticalLineLeft[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte horizontaal[8] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte temp[8] = {
  B01110,
  B10001,
  B10001,
  B01010,
  B01010,
  B01010,
  B01010,
  B00100
};

byte temp1[8] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B10001,
  B10001,
  B01110
};

void setup() {
  //pinMode(interruptPin, INPUT);
  //attachInterrupt(13, handleInterrupt, CHANGE ); //Interupt pin
  Serial.begin(115200);
  //  mySerial.begin(4800);

  while (!Serial) {}; // wait for serial port to connect. Needed for native USB port only

  Blynk.begin(auth, ssid, pass);

  Wire.begin(4, 5);   //Define the i2c pins on the ESP8266  SDA, GPIO4 = D2     SCL, GPIO5 = D1

  lcd.begin(20, 4);
  lcd.createChar(0, verticalLine);
  lcd.createChar(1, verticalLineLeft);
  lcd.createChar(3, horizontaal);
  lcd.createChar(4, temp);
  lcd.createChar(5, temp1);

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
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 1);

  Blynk.virtualWrite(V0, DHT11.temperature);
  Blynk.virtualWrite(V3, lux);

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
  else {
    digitalWrite(ledPin2, LOW);
  }
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  DrawLcd();
}



void DrawLcd() {
  lcd.setCursor(0, 1);

  for (int i = 0; i <= 19; i++) {
    lcd.write(byte(3));
  }

  lcd.setCursor(0, 2);
  lcd.print("____________________");
  lcd.setCursor(14, 4);
  lcd.write(byte(0));
  lcd.setCursor(15, 4);
  lcd.print("  ");
  lcd.setCursor(15, 4);
  lcd.print(currentHour);
  lcd.setCursor(17, 4);
  lcd.print(":");
  lcd.setCursor(18, 4);
  lcd.print("  ");
  lcd.setCursor(18, 4);
  lcd.print(currentMinutes);



  lcd.setCursor(0, 4);
  lcd.print("LUX");
  lcd.setCursor(4, 4);
  lcd.print("     ");
  lcd.setCursor(4, 4);
  lcd.print(lux);
  lcd.write(byte(1));

  if (lichtsOnOff == 1) {
    lcd.setCursor(16, 0);
    //lcd.print("lights:");
    lcd.print("ON ");
  }
  else {
    lcd.setCursor(16, 0);
    //lcd.print("lights:");
    lcd.print("OFF");
  }




  if (autoMode == 1 ) {
    lcd.setCursor(0, 0);
    lcd.print("AutoMode ");
    lcd.setCursor(9, 0);
    lcd.print("ON ");
    lcd.write(byte(1));
  }

  else {
    lcd.setCursor(0, 0);
    lcd.print("AutoMode ");
    lcd.setCursor(9, 0);
    lcd.print("OFF");
    lcd.write(byte(1));
  }
  lcd.setCursor(8, 3);
  lcd.print((float)DHT11.temperature, 0);
  lcd.print((char)223);
  //  lcd.write(byte(5));
  lcd.setCursor(11, 3);
  lcd.print((float)DHT11.humidity, 0);
  lcd.print("%");


}


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
  for (int i = 0; i <= 7; i++) {
    pwm.setPin(i, 4096 );//setPWM(channel, on, off)
  }
  for (int j = 8; j <= 11; j++) {
    pwm.setPin(j, 0 );//setPWM(channel, on, off)
  }
  lichtsOnOff = 1;
}


void turnAllOff() {
  for (int i = 0; i <= 7; i++) {
    pwm.setPin(i, 0 );//setPWM(channel, on, off)
  }
  for (int j = 8; j <= 11; j++) {
    pwm.setPin(j, 4096 );//setPWM(channel, on, off)
  }
  lichtsOnOff = 0;
}






BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  
  if (pinValue == 1) {
    lichtsOnOff = 1;
    turnAllOn();
  }
  else {
    lichtsOnOff = 0;
    turnAllOff();
  }
  yield();
}




BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  for (int i = 0; i <= 4; i++) {
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
  for (int i = 0; i <= 7; i++) {
    pwm.setPin(i, 4096 );//setPWM(channel, on, off)
    delay(150);
  }

  for (int i = 0; i <= 7; i++) {
    pwm.setPin(i, 0 );//setPWM(channel, on, off)
    delay(150);
  }


  delay(10);

  for (int i = 0; i <= 50; i++) {
    delay(40);
    for (int i = 0; i <= 7; i++) {
      pwm.setPin(i, 4096 );//setPWM(channel, on, off)
      delay(0);
    }
    delay(40);

    for (int i = 0; i <= 7; i++) {
      pwm.setPin(i, 0 );//setPWM(channel, on, off)
      delay(0);
    }
  }


}




BLYNK_WRITE(V5) {
  autoMode = param.asInt();
}



BLYNK_WRITE(V7) {
  int pinValue = param.asInt();
  for (int i = 0; i <= 2; i++) {
    pwm.setPin(i, pinValue );//setPWM(channel, on, off)
  }
  yield();
}

BLYNK_WRITE(V8) {
  int pinValue = param.asInt();
  for (int i = 3; i <= 4; i++) {
    pwm.setPin(i, pinValue );//setPWM(channel, on, off)
  }
  yield();
}


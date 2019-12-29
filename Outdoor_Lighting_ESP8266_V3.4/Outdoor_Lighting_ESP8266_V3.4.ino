//_______________include libraries here._______________
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_PWMServoDriver.h>
#include <BH1750.h>
#include <SensorUtil.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <dht11.h>
#include <SimpleTimer.h>
//_____________________________________________________




//________________define stuff here.___________________
#define lightSensorAdress 0x23     // I2C addres of Light sensor.                            Default =  0x23.
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // I2C addres of PWM breakout.      Default =  0x40.
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address   Default =  0x27.
//#define BLYNK_PRINT Serial       // Debugging for blynk, turn of when not needed.
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 3600;   //Winter (3600)  Zomer(7200).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", utcOffsetInSeconds);
BH1750 lightMeter;
#define DHT11PIN D4
dht11 DHT11;
SimpleTimer timer;
WidgetLED led1(V9);

//____________________________________________________




//____________________All variables____________________
int val = 0;
int pinValue = 0;
int pinData = 0;
int i = 0;
int lux = 0;
int autoMode = 1;
int currentHour;
int currentMinutes;
int currentDay;
int ledPin = D6;  //licht for RST. (White LED)
int ledPin2 = D7; //licht for auto mode.(Red LED)
int nRF24 = D8;
int lichtsOnOff = 0;
int k = 0;
int tempWarning = 0;




//____________________________________SETTINGS________________________________________
// Blynk settings
char auth[] = "auth here";  //Auth key.
char ssid[] = "ssid here";               //WIFI SSID it connects with.
char pass[] = "pasword here";                     //WIFI password.

//Min Lux, is when the light go ON.
int minLux = 1;
//Max Lux, is when the light go OFF.
int maxLux = 15;


//=============Vrijdag T/M Zaterdag==============
//Defining start day time:
int startHour = 2;  //Lights go out at 1:59./
//Defining end day time:
int endHour = 6; //Lights turn on at 6:59.
//===============================================


//=============Zondag T/M Donderdag=============
//Defining start day time:
int startHour1 = 0;  //Lights go out at 23:59./
//Defining end day time:
int endHour1 = 6; //Lights turn on at 6:59.
//===============================================
//____________________________________End SETTINGS_______________________________________




//Bitmap IMG
byte temp[8] = {
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte vocht[8] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110
};




void setup() {
  //Serial.begin(115200); // For debugging.
  //while (!Serial) {}; // wait for serial port to connect. Needed for native USB port only.
  Blynk.begin(auth, ssid, pass); //ESP8266 connects to WiFi.
  timer.setInterval(4000L, secondTimer); //Time length of the 2nd timer.

  //The I2c bus starts on the indicated pins.
  Wire.begin(4, 5);   //Define the i2c pins on the ESP8266    SDA, GPIO4 = D2___SCL, GPIO5 = D1.

  lcd.begin(20, 4); //Start the I2c LCD screen.
  lcd.createChar(1, temp);
  lcd.createChar(2, vocht);

  lightMeter.begin(BH1750::ONE_TIME_LOW_RES_MODE);  //Begins the sensor en sets it to the LowRes Mode.
  pwm.begin();
  pwm.setPWMFreq(1600); // This is the maximum PWM frequency.
  timeClient.begin(); //Start communication with the time server to retrieve the time.

  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(nRF24, OUTPUT);
  Blynk.notify("Buiten verlichting is ACTIEF."); //Sends a notification to your phone that the system is active.
}




void loop() {
  Blynk.run(); //Communication with the app.
  timer.run(); //2nd timer to time stuff.

  //Writes temperature and LUX data to the app.
  Blynk.virtualWrite(V0, DHT11.temperature);
  Blynk.virtualWrite(V3, lux);



  //____________Auto Mode____________
  if (autoMode == 1 ) {
    digitalWrite(ledPin2, HIGH); //Turns ON the LED to see that Auto Mode is ON.
    Blynk.virtualWrite(V5, 1); //turns on the button in the app.





    //=============================Weekend=============================
    if (currentDay == 6 || currentDay == 0) {
      if (lux < minLux ) {
        if (currentHour >= startHour && currentHour <= endHour ) {
          turnAllOff();
          pwm.setPin(10, 4096 );//setPWM(channel, on, off).
          //        pwm.setPin(9, 4096 );//setPWM(channel, on, off).
        }
        else {
          turnAllOn();
        }
      }
      else if (lux > maxLux) {
        turnAllOff();
      }
    }

    //=========================During the week===========================
    else {
      if (lux < minLux ) {
        if (currentHour >= startHour1 && currentHour <= endHour1 ) {
          turnAllOff();
          pwm.setPin(10, 4096 );//setPWM(channel, on, off).
          //        pwm.setPin(9, 4096 );//setPWM(channel, on, off).
        }
        else {
          turnAllOn();
        }
      }
      else if (lux > maxLux) {
        turnAllOff();
      }
    }
    //====================================================================
  }
  
  else {
    digitalWrite(ledPin2, LOW);//Turns OFF the LED to see that Auto Mode is OFF.
    Blynk.virtualWrite(V5, 0); //turns on the button in the app.
  }
}





void secondTimer() {
  getTime();                           //Get Time.
  DrawLcd();                           //Draws the LCD.
  int chk = DHT11.read(DHT11PIN);      //Get the Temperature from DHT11.
  lux = lightMeter.readLightLevel();   //Get the Lux from BH1750.



  //lets the status LED flash.
  //----------------------------------
  if (autoMode == 0 ) {
    digitalWrite(ledPin2, HIGH);
    delay(100);
    digitalWrite(ledPin2, LOW);
    delay(100);
  }

  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  //----------------------------------



  //If auto mode is still off by morning, than turn it on again.
  //-------------------------------------------------------------
  if (autoMode == 0 ) {
    if (currentHour == endHour) {
      autoMode = 1;
    }
  }
  //-------------------------------------------------------------



  //if the temperature becomes too high, a message is sent.
  //-------------------------------------------------------------
  if  (tempWarning == 0) {
    if (DHT11.temperature > 45) {
      Blynk.notify("WAARSCHUWING!   Tempratuur boven 45°C, Kontroleer systeem.");
      tempWarning = 1;
    }
  }
  if (DHT11.temperature <= 42) {
    tempWarning = 0;
  }
  //-------------------------------------------------------------
}


//display stuff.
void DrawLcd() {

  //writes the time to the screen.
  //-------------------------------------------------------------
  lcd.setCursor(0, 0);
  lcd.print(currentHour);
  lcd.setCursor(2, 0);
  lcd.print(":");
  lcd.setCursor(3, 0);
  lcd.print("  ");
  lcd.setCursor(3, 0);
  lcd.print(currentMinutes);
  lcd.setCursor(9, 1);
  //-------------------------------------------------------------




  //LCD Draw Day of the week.
  //-------------------------------------------------------------
  lcd.setCursor(0, 1);
  if (currentDay == 0) {
    lcd.print("Zondag   ");
  }
  else if (currentDay == 1) {
    lcd.print("Maandag  ");
  }
  else if (currentDay == 2) {
    lcd.print("Dinsdag  ");
  }
  else if (currentDay == 3) {
    lcd.print("Woensdag ");
  }
  else if (currentDay == 4) {
    lcd.print("Donderdag  ");
  }
  else if (currentDay == 5) {
    lcd.print("Vrijdag  ");
  }
  else if (currentDay == 6) {
    lcd.print("Zaterdag ");
  }
  //-------------------------------------------------------------




  lcd.setCursor(10, 1);
  lcd.print("LUX");
  lcd.setCursor(13, 1);
  lcd.print("       ");
  lcd.setCursor(13, 1);
  lcd.print(lux);

  if (lichtsOnOff == 1) {
    lcd.setCursor(0, 3);
    lcd.print("Lights:  ON");
  }
  else {
    lcd.setCursor(0, 3);
    lcd.print("Lights: OFF");
  }

  if (autoMode == 1 ) {
    lcd.setCursor(0, 2);
    lcd.print("Auto:    ON");
  }
  else {
    lcd.setCursor(0, 2);
    lcd.print("Auto:   OFF");
  }
  lcd.setCursor(11, 0);
  lcd.write(byte(1));
  lcd.setCursor(12, 0);
  lcd.print((float)DHT11.temperature, 0);
  lcd.print((char)223);
  lcd.setCursor(16, 0);
  lcd.write(byte(2));
  lcd.print((float)DHT11.humidity, 0);
  lcd.print("%");
}




//retrieved time from time server.
//-------------------------------------------------------------
void getTime() {
  timeClient.update();
  currentHour = timeClient.getHours();
  currentMinutes = timeClient.getMinutes();
  currentDay = timeClient.getDay();
}
//-------------------------------------------------------------




//turn everything on.
//-------------------------------------------------------------
void turnAllOn() {
  for (int i = 0; i <= 7; i++) {
    pwm.setPin(i, 4096 );//setPWM(channel, on, off).
  }
  for (int j = 8; j <= 11; j++) {
    pwm.setPin(j, 0 );//setPWM(channel, on, off).
  }
  lichtsOnOff = 1;
  Blynk.virtualWrite(V1, 1);
  led1.on();
  digitalWrite(nRF24, HIGH);
}
//-------------------------------------------------------------




//turn everything off.
//-------------------------------------------------------------
void turnAllOff() {
  for (int i = 0; i <= 7; i++) {
    pwm.setPin(i, 0 );//setPWM(channel, on, off)
  }
  for (int j = 8; j <= 11; j++) {
    pwm.setPin(j, 4096 );//setPWM(channel, on, off)
  }
  lichtsOnOff = 0;
  Blynk.virtualWrite(V1, 0);
  led1.off();
  digitalWrite(nRF24, LOW);
}
//-------------------------------------------------------------




//blynk app stuff.
//-------------------------------------------------------------
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

BLYNK_WRITE(V4) {
  for (int i = 0; i <= 3; i++) {
    for (int i = 0; i <= 7; i++) {
      pwm.setPin(i, 4096 );//setPWM(channel, on, off)
      delay(150);
    }
    for (int i = 0; i <= 7; i++) {
      pwm.setPin(i, 0 );//setPWM(channel, on, off)
      delay(150);
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

BLYNK_WRITE(V10) {
  int pinValue = param.asInt();
  pwm.setPin(5, pinValue );//setPWM(channel, on, off)
  yield();
}

BLYNK_WRITE(V11) {
  int pinValue = param.asInt();
  pwm.setPin(6, pinValue );//setPWM(channel, on, off)
  yield();
}

BLYNK_WRITE(V12) {
  int pinValue = param.asInt();
  pwm.setPin(8, pinValue );//setPWM(channel, on, off)
  pwm.setPin(9, pinValue );//setPWM(channel, on, off)
  pwm.setPin(10, pinValue );//setPWM(channel, on, off)
  pwm.setPin(11, pinValue );//setPWM(channel, on, off)
  yield();
}

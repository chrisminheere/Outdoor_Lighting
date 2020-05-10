#include <Wire.h>
#include <TimerOne.h>
#include <Door.h>
#include <SensorUtil.h>
#include <OutsideLight.h>

#define doorSwitch 18 //This should be one of the following interrupt pins: 2,3,18,19,20,21
#define motionSensor 19 //This should be one of the following interrupt pins: 2,3,18,19,20,21
#define doorLight 4
#define outsideLight1 22
#define outsideLight2 24
#define outsideLight3 26
#define outsideLight4 28


//Defining start day time:
int startHour = 5;
//Defining end day time:
int endHour = 2;
//Defining darkLux:
int darkLux = 9;
//Defining lightLux:
int lightLux = 12;

//Defining I2C adresses:
#define clockModuleAdress 0x68 //Clock module
#define lightSensorAdress 0x23 //Light sensor

//Defining library classes:
Door door = Door(doorLight);
SensorUtil util = SensorUtil(clockModuleAdress, lightSensorAdress);
OutsideLight light1 = OutsideLight(outsideLight1);
OutsideLight light2 = OutsideLight(outsideLight2);
OutsideLight light3 = OutsideLight(outsideLight3);
OutsideLight light4 = OutsideLight(outsideLight4);


//Poll counter:
int pollCounter = 0;
int pollInterveral = 1;
int currentHour;

void setup() {
  //Setting pinModus:
  pinMode(doorSwitch, INPUT);
  pinMode(motionSensor, INPUT);
  pinMode(doorLight, OUTPUT);
  pinMode(outsideLight1, OUTPUT);
  pinMode(outsideLight2, OUTPUT);
  pinMode(outsideLight3, OUTPUT);
  pinMode(outsideLight4, OUTPUT);
  //Turning off lights:
  light1.turnOff();
  light2.turnOff();
  light3.turnOff();
  light4.turnOff();
  //Initializing light sensor:
  util.initializeLightSensor();
  //Attaching interrupts:
  attachInterrupt(digitalPinToInterrupt(doorSwitch), doorEvent, CHANGE); //Door open/close.
  attachInterrupt(digitalPinToInterrupt(motionSensor), motionSensorEvent, CHANGE); //Motion sensor
  //Sarting timer:
  Timer1.initialize(10000); //Updates every second.
  Timer1.attachInterrupt(timerEvent);

}

int hour;


void loop() {
  //Checking light intensity:
  if (pollCounter % (pollInterveral * 85) == 0) {
    currentHour = util.getCurrentHour();
    if (!(currentHour <= startHour && currentHour >= endHour))
      handleLightSensor();
  }
  delay(10);
  pollCounter++;
}

//Triggerd when door state changes.
void doorEvent() {
  door.handleDoorEvent(digitalRead(doorSwitch));
}

//Triggered when motion is detected.
void motionSensorEvent() {
  bool motion = digitalRead(motionSensor);
  util.handleMotionSensorEvent(motion);
  if (currentHour <= startHour && currentHour >= endHour) {
    if (motion) {
      light1.turnOn();
      light2.turnOn();
      light3.turnOn();
      light4.turnOn();
    }
    else {
      light1.turnOff();
      light2.turnOff();
      light3.turnOff();
      light4.turnOff();
    }
  }
}

//Timer for effects.
void timerEvent() {
  if (door.getDoorLedFading()) //Door led fading
    door.fadeLed();
}

void handleLightSensor() {
  uint16_t lux = util.getLightSensorLux();
  if (lux <= darkLux) { //Turn on lights
    light1.turnOn();
    light2.turnOn();
    light3.turnOn();
    light4.turnOn();
  }
  else if (lux >= lightLux) {
    light1.turnOff();
    light2.turnOff();
    light3.turnOff();
    light4.turnOff();
  }
}

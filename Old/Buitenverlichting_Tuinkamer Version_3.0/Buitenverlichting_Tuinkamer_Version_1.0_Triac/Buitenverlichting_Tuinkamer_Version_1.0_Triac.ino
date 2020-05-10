//#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

unsigned char dimming = 45;  // Dimming level (0-100)


char auth[] = "1a7a3e1e7e1446c7a8ce647142957ba5"; //6229d43d21384992a951f21cd69db711

// Set password to "" for open networks.
char ssid[] = "Fam-Minheere 2.4GHz";
char pass[] = "Minheere@asus";

void setup()
{
  
  Blynk.begin(auth, ssid, pass);
  pinMode(12, OUTPUT);// Set AC Load pin as output
  attachInterrupt(13, zero_crosss_int, RISING); //Interupt pin
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


void loop()
{
  Blynk.run();
}

BLYNK_WRITE(V6)
{
  dimming = param.asInt(); // Get value as integer
}

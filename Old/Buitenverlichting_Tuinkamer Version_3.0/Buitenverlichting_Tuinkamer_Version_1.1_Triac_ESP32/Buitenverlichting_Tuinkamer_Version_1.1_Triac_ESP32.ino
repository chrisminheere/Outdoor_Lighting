//#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


unsigned char dimming = 45;  // Dimming level (0-100)
char auth[] = "1a7a3e1e7e1446c7a8ce647142957ba5";


char ssid[] = "Fam-Minheere 2.4GHz";
char pass[] = "Minheere@asus";

void setup()
{
 
  //Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  pinMode(12, OUTPUT);// Set AC Load pin as output
  attachInterrupt(14, zero_crosss_int, RISING); //Interupt pin
}

void loop()
{
  Blynk.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  
}


BLYNK_WRITE(V6)
{
  dimming = param.asInt(); // Get value as integer
  Serial.println(dimming);
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

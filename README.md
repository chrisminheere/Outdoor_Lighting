# Outdoor_Lighting
 Home Automation

| PinOut.   |	Pin On ESP8266   |	Pin In Code   |	Function   |	I2C Address   |	RJ45   |	Technical specs   |
| ntpUDP timeClient   |   ---   |		|   Software  europe.pool.ntp.org   |	Gets real-time Time from the Internet   |   ---   |   ---    |   ---    |
 			
						
| BH1750 	SDA, D1 	SDA, GPIO4 	measure light	0x23	7 & 8	In - 3.3V - 5V
| level Out - 3.3V - 5V
| 16bit ADC
| BH1750 	SCL, D2	 SCL, GPIO5	measure light	0x23	7 & 8	In - 3.3V - 5V
| level Out - 3.3V - 5V
| 16bit ADC
						
| PCA9685	SDA, D1 	SDA, GPIO4	16-Channel PWM Driver	0x40	7 & 8	In & Out 3.3V - 6V
| 12-bit DAC
| PCA9685	SCL, D2	 SCL, GPIO5	16-Channel PWM Driver	0x40	7 & 8	In & Out 3.3V - 6V
| 12-bit DAC
						
| HD44780	SDA, D1	SDA, GPIO4	LCD Display	0x27	7 & 8	
| HD44780	SCL, D2	SCL, GPIO5	LCD Display	0x27	7 & 8	
						
| SDR-05VDC-SL-C			Relay Board			In - 3.3V - 5V
| Out – 230V 16A
						
| PIR Motion Sensor	D5	GPIO 14	measures movement		3	In 5V - 20V
| level out: 3.3V high, low 0V
						
| DHT11	D4 	GPIO 2	Digital Temperature and Humidity Sensor		5?	In - 3.3V - 5V
| level Out - 3.3V - 5V 
						
| RST button	RST		resets the esp8266			3.3V

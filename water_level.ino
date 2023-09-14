#include <SoftwareSerial.h>
SoftwareSerial GSM(9, 10); // RX, TX pins for GSM module (change pins as needed)

/* Change these values based on your calibration values */
int lowerThreshold = 420;
int upperThreshold = 520;

// Sensor pins
#define sensorPower 7
#define sensorPin A0

// Value for storing water level
int val = 0;

// Declare pins to which LEDs are connected
int redLED = 2;
int yellowLED = 3;
int greenLED = 4;

void setup() {
	Serial.begin(9600);
   GSM.begin(9600);
  
	pinMode(sensorPower, OUTPUT);
	digitalWrite(sensorPower, LOW);
	
	// Set LED pins as an OUTPUT
	pinMode(redLED, OUTPUT);
	pinMode(yellowLED, OUTPUT);
	pinMode(greenLED, OUTPUT);

	// Initially turn off all LEDs
	digitalWrite(redLED, LOW);
	digitalWrite(yellowLED, LOW);
	digitalWrite(greenLED, LOW);
}
void sendSMS(String message) {
    // Send an SMS using AT commands
    GSM.println("AT+CMGF=1"); // Set SMS text mode
    delay(1000);
    GSM.println("AT+CMGS=\"+1234567890\""); // Replace with the recipient's phone number
    delay(1000);
    GSM.println(message);
    delay(1000);
    GSM.println((char)26); // Send Ctrl+Z to indicate the end of the message
    delay(1000);
}


void loop() {
    int level = readSensor();

    if (level == 0) {
        // Water is empty, send an SMS
        sendSMS("Water Level: Empty");
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
    } else if (level > 0 && level <= lowerThreshold) {
        // Water level is low, send an SMS
        sendSMS("Water Level: Low");
        Serial.println("Water Level: Low");
        digitalWrite(redLED, HIGH);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, LOW);
    } else if (level > lowerThreshold && level <= upperThreshold) {
        Serial.println("Water Level: Medium");
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, HIGH);
        digitalWrite(greenLED, LOW);
    } else if (level > upperThreshold) {
        // Water level is high, send an SMS
        sendSMS("Water Level: High");
        Serial.println("Water Level: High");
        digitalWrite(redLED, LOW);
        digitalWrite(yellowLED, LOW);
        digitalWrite(greenLED, HIGH);
    }
    delay(1000);
}


//This is a function used to get the reading
int readSensor() {
	digitalWrite(sensorPower, HIGH);
	delay(10);
	val = analogRead(sensorPin);
	digitalWrite(sensorPower, LOW);
	return val;
}

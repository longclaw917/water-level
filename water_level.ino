#include <SoftwareSerial.h>

SoftwareSerial GSM(7, 8);  // RX, TX pins for GSM module
const int waterSensorPin = A0; // Analog pin to connect the water level sensor

void setup() {
  // Initialize serial communication with Arduino IDE
  Serial.begin(9600);

  // Initialize serial communication with the GSM module
  GSM.begin(9600);

  // Configure the water sensor pin as an input
  pinMode(waterSensorPin, INPUT);

  // Give some time for the GSM module to initialize
  delay(2000);
}

void loop() {
  // Read the water level sensor value
  int waterLevel = analogRead(waterSensorPin);

  // You may need to adjust the threshold value based on your sensor
  int threshold = 500;

  // Check if the water level is above the threshold
  if (waterLevel > threshold) {
    // Water level is high, send an SMS
    sendSMS("Water level is high! Current level: " + String(waterLevel));
  }

  // Delay between readings (adjust as needed)
  delay(60000); // 1 minute delay
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

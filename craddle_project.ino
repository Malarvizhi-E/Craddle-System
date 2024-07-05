#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Define pins
const int soundSensorPin = A0;
const int wetSensorPin = A1;
const int buzzerPin = 8;
const int servoPin = 9;  // Servo motor connected to pin 9

// Define thresholds
const int soundThreshold = 500; // Adjust based on your sound sensor's output
const int wetThreshold = 300;   // Adjust based on your wet sensor's output

// Initialize LCD with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the I2C address if necessary

// Create a servo object
Servo servoMotor;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize buzzer pin as output
  pinMode(buzzerPin, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight(); // If your LCD has backlight control

  // Attach the servo motor to the specified pin
  servoMotor.attach(servoPin);
  servoMotor.write(90);  // Start with servo in neutral position (90 degrees)
}

void loop() {
  // Read the sensor values
  int soundLevel = analogRead(soundSensorPin);
  int wetLevel = analogRead(wetSensorPin);

  // Print sensor values to serial monitor for debugging
  Serial.print("Sound Level: ");
  Serial.println(soundLevel);
  Serial.print("Wet Level: ");
  Serial.println(wetLevel);

  // Flags to track the status of sensors
  bool isWet = false;
  bool isCrying = false;

  // Check if the wet level is above the threshold
  if (wetLevel > wetThreshold) {
    isWet = true;
  }

  // Check if the sound level is above the threshold
  if (soundLevel > soundThreshold) {
    isCrying = true;
  }

  // If the diaper is wet, sound the buzzer and display message
  if (isWet) {
    digitalWrite(buzzerPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Diaper Wet!");
  }

  // If the baby is crying, sound the buzzer, start the servo, and display message
  if (isCrying) {
    
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Baby Crying!");

    for(int i=0;i<10;i++) {// Swing the cradle using the servo motor
    for (int pos = 60; pos <= 120; pos += 1) { // Move from 60 to 120 degrees
      servoMotor.write(pos);
      delay(5); // Adjust speed of the swing
    }
    for (int pos = 120; pos >= 60; pos -= 1) { // Move back from 120 to 60 degrees
      servoMotor.write(pos);
      delay(5); // Adjust speed of the swing
    }
  }
  }
  // If either condition is met, keep active for a certain period
  if (isWet || isCrying) {
    delay(2000); // Keep active for 5 seconds

    // Stop the servo and buzzer after the delay
    servoMotor.write(90); // Move servo back to neutral position
    digitalWrite(buzzerPin, LOW);
  }

  // Default message on LCD when no conditions are met
  if (!isWet && !isCrying) {
    lcd.clear();
    lcd.print("Cradle System");
    lcd.setCursor(0, 1);
    lcd.print("Monitoring...");
  }

  // Short delay before the next reading
  delay(100);
}

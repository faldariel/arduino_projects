/*
 * A button click starts or stops measurements for humidity and 
 * temperature (°C), outputting them to a LCD Display.
 * 
 * For the display, Android library LiquidCrystal was used 
 * https://www.arduino.cc/en/Reference/LiquidCrystal
 * 
 * The sensor used is a DHT11, which uses an additional non-standard 
 * Arduino library. It is using a simpler library than the standard
 * DHT library, available for download from github:
 * https://github.com/faldariel/arduino_projects/tree/master/humidityTempSensorDHT114pins
 * 
 * This example uses DHT11 with a 4.75 ohm resistor and a standard 
 * button with a 220 ohm resistor, along with a 16x2 LCD Display
 * using a potentiometer and one 220ohm resistor.
 */

#include <dht11.h>
#include <LiquidCrystal.h>
#define DHT11PIN 8

dht11 DHT11;
LiquidCrystal lcd(12, 11, 2, 3, 4, 5); // RS, E (enable), d4, d5, d6, d7 

const int buttonPin = 10; // Digital pin for the standard button.
boolean readingDhtActive = false; // The button has activated or deactivated readings from DHT11.
boolean lastButtonState = LOW;
unsigned long previousMilliSec = 0;
const unsigned long readingDhtInterval = 4000; // How long to wait between each reading from DHT11.

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT); // Sets the pin to input
  lcd.begin(16, 2); // 16 columns, 2 rows of LCD Display.
  
  // First row of LCD Display.
  lcd.setCursor(0,0);
  lcd.print("Click to start");
  // Second row of LCD Display.
  lcd.setCursor(0,1);
  lcd.print("measurement");
}

void loop()
{
  int currentButtonState = digitalRead(buttonPin); // Check if button is pressed or not

  // When a button is pressed and released, the button will go from HIGH to LOW. We only want to
  // react to this scenario since we want the button to toggle the reading of DHT11 on and off.
  if ((currentButtonState == LOW) && (lastButtonState == HIGH)) { 
    delay (10); // Buttons are subject to 'rebounce' giving junk values so we wait until they pass.
    readingDhtActive = !readingDhtActive; // Toggling the reading of DHT11 on and off.
    
    if (readingDhtActive) {
      lcd.clear(); // Clear the display from old content.
      lcd.setCursor(0,0);
      lcd.print("Starting");
      lcd.setCursor(0,1);
      lcd.print("measurement...");
      delay (2000); // Give the User a chance to read.
    } else {
      lcd.clear(); // Clear the display from old content.
      lcd.setCursor(0,0);
      lcd.print("Ending");
      lcd.setCursor(0,1);
      lcd.print("measurement");
    }
  }
  unsigned long currentMilliSec = millis();

  // Check the interval to see if it is time to update values read.
  if ((readingDhtActive == true) && ((currentMilliSec - previousMilliSec) > readingDhtInterval)) {
    DHT11.read(DHT11PIN);
    lcd.clear(); // Clear the display from old content.
    
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print(DHT11.temperature);
    lcd.print((char)223); // Symbol for °
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print(DHT11.humidity);
    lcd.print("%");

    previousMilliSec = currentMilliSec;
  }
  lastButtonState = currentButtonState; // Set the lastButtonState so that we can check it 
                                        // against a new reading next iteration.
}

/*
 * A button click starts or stops measurements for humidity and 
 * temperature (°C).
 * 
 * The sensor used is a DHT11, which uses an additional non-standard 
 * Arduino library. It is using a simpler library than the standard
 * DHT library, available for download from github:
 * https://github.com/faldariel/arduino_projects/tree/master/humidityTempSensorDHT114pins
 * 
 * This example uses DHT11 with a 4.75 ohm resistor and a standard 
 * button with a 220 ohm resistor.
 */

#include <dht11.h>
#define DHT11PIN 4

dht11 DHT11;

const int buttonPin = 2; // Digital pin for the standard button.
boolean readingDhtActive = false; // The button has activated or deactivated readings from DHT11.
boolean lastButtonState = LOW;
unsigned long previousMilliSec = 0;
const unsigned long readingDhtInterval = 4000; // How long to wait between each reading from DHT11.

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT); // Sets the pin to input
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
      Serial.print("Starting measurement\n");
    } else {
      Serial.print("Ending measurement\n");
    }
  }
  unsigned long currentMilliSec = millis();

  // Check the interval to see if it is time to update values read.
  if ((readingDhtActive == true) && ((currentMilliSec - previousMilliSec) > readingDhtInterval)) {
    Serial.println();
    DHT11.read(DHT11PIN);

    Serial.print("Humidity (%): ");
    Serial.println((float)DHT11.humidity, 2);

    Serial.print("Temperature (C): ");
    Serial.println((float)DHT11.temperature, 2);

    previousMilliSec = currentMilliSec;
  }
  lastButtonState = currentButtonState; // Set the lastButtonState so that we can check it 
                                        // against a new reading next iteration.
}

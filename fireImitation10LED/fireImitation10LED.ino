/*
 * Imitate a fire's flickering light using PWD and NO PWD LED.
 * 
 * PWD LEDs will dim and brighten while LED without PWM support flickers.
 * 
 * This example used 10 3mm LED, each with a 220 ohm resistor connected to
 * individual pins.
 * 
 * No external libraries needed.
*/

// Setup values for LED on pins with PWM (Pulse Width Monitoring) support.
const int numOfPwmLeds = 6;
const int pwmLedPins[numOfPwmLeds] = {3, 5, 6, 9, 10, 11}; // Pins for the LEDs with PWM support.
int Brightness[numOfPwmLeds] = {240, 127, 63, 191, 0, 127}; // Starting value for led brightness.
int Direction[numOfPwmLeds] = {1, 1, -1, 1, 1, -1}; // Indicates if the LED should brighten or dim.

// Duration for light on/off for LED on pins without PWM support. Used in schematics below.
const int MAX_LIGHT_DURATION = 150;
const int MEDIUM_LIGHT_DURATION = 40;
const int MIN_LIGHT_DURATION = 20;
const int GLIMMER = 1;
const int NOX = 1;

// Setup values for LED on pins without PWD support.
const int numOfNoPwmLeds = 4;
const int NoPwmLedPins[numOfNoPwmLeds] = {4, 7, 8, 12}; // Pins for the LEDs without PWM.
bool Shine[numOfNoPwmLeds] = {true, true, true, true}; // Indicates if the LED should be lit or not.
int currentIndex[numOfNoPwmLeds] = {0, 0, 0, 0}; // Stores the current index of ShineDurationSchematics for each LED without PWM.
int currentShineDuration[numOfNoPwmLeds] = {NOX, NOX, NOX, NOX}; // Stores the shine duration from ShineDurationSchematics schematic for each NO PWM LED

/*
 * Light sequences where NOX is used to indicate light OFF. It seems array pointers cannot be used due to limitation in compiler.
 * Each LED has it's own schematic to simulate the randomness of a flickering flame. (Odd index signifies OFF if Shine starts as true).
*/
int numOfIndexInSchematic = 0; // Scematic may differ in number of elements
const int MAX_CYCLES_SIZE = 12; // Maximum number of values in a schematic
const int ShineDurationSchematics[][MAX_CYCLES_SIZE] = 
          {{MIN_LIGHT_DURATION, NOX, GLIMMER, NOX, MIN_LIGHT_DURATION, NOX, MAX_LIGHT_DURATION, NOX, MEDIUM_LIGHT_DURATION, NOX, MIN_LIGHT_DURATION, NOX},
           {MAX_LIGHT_DURATION, NOX, MEDIUM_LIGHT_DURATION, NOX, GLIMMER, NOX, GLIMMER, NOX, MEDIUM_LIGHT_DURATION, NOX, MEDIUM_LIGHT_DURATION, NOX},
           {MAX_LIGHT_DURATION, NOX, GLIMMER, NOX, GLIMMER, NOX, MAX_LIGHT_DURATION, NOX, MEDIUM_LIGHT_DURATION, NOX, MIN_LIGHT_DURATION, NOX},
           {MIN_LIGHT_DURATION, NOX, MAX_LIGHT_DURATION, NOX, MAX_LIGHT_DURATION, NOX, GLIMMER, NOX, GLIMMER, NOX, MAX_LIGHT_DURATION, NOX}};


// Other
int i = 0;
int j = 0;

void setup() {
  Serial.begin(9600);
  for (i = 0; i < numOfPwmLeds; i++) {
    pinMode(pwmLedPins[i], OUTPUT); // Sets the pins to output
  }
  for (j = 0; j < numOfNoPwmLeds; j++) {
    pinMode(NoPwmLedPins[j], OUTPUT); // Sets the pins to output
  }
}


void loop() {
// Dims and brightens LED on pins with PWM support.
  for (i = 0; i < numOfPwmLeds; i++) { // Loop through all PWM LEDs
    analogWrite(pwmLedPins[i], Brightness[i]); // Write to output pin
    Brightness[i] = Brightness[i] + Direction[i] * 5; // Change brightness (brighten or dim)
    if (Brightness[i] >= 255) { // Change direction if brightness hits the highest value (start dimming)
      Brightness[i] = 255;
      Direction[i] = -1;
    }
    if (Brightness[i] <= 0) { // Change direction if brightness hits the lowest value (start brighten)
      Brightness[i] = 0;
      Direction[i] = 1;
    }
  }

// Makes LED on pins without PWM support flicker.
  for (j = 0; j < numOfNoPwmLeds; j++) { // Loop through all No-PWM LEDs
    if (currentShineDuration[j] <= 0) { 
      toggleNoPwmLed(j); // Turn the LED on or off
      numOfIndexInSchematic = (sizeof(ShineDurationSchematics[j])/sizeof(ShineDurationSchematics[j][currentIndex[j]]));
      currentIndex[j] = nextIndex(currentIndex[j], numOfIndexInSchematic); //If Shine Duration has hit 0 we move to the next step in the schematic.
      currentShineDuration[j] = ShineDurationSchematics[j][currentIndex[j]];
    } else {
      currentShineDuration[j]--;
    }
  }
  delay (30); // Time delay until it is all done again
  Serial.println();
}

void toggleNoPwmLed(int pin) {
  if (Shine[pin]) {
    digitalWrite(NoPwmLedPins[pin], LOW);
  } else {
    digitalWrite(NoPwmLedPins[pin], HIGH);
  }
  Shine[pin] = !Shine[pin];
}

int nextIndex(int currentIndexNum, int numOfIndex) {
  if (currentIndexNum < numOfIndex) {
      currentIndexNum++;
  } else {
    currentIndexNum = 0;
  }
  return currentIndexNum;
}

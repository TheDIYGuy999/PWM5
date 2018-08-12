/* This PWM solar charge controller is based on:
    Julian Itett's PWM5: https://www.youtube.com/watch?v=AobcNhLG_Xw&list=PLjzGSu1yGFjWa5BEHgX5UUrSexppett3W
    arduined.eu's PWM solar charge controller: https://www.arduined.eu/arduino-solar-charge-controller/

    Changes and improvements, implemented by TheDIYGuy999:
    - Using the 3.3V, 8MHz Arduino Pro Mini, which requires about 5mA (instead of 20mA for the 5V, 16MHz version)
    - IRLZ44N logic level MOSFET used, because the charge pump can't deliver enough voltage with 3.3V supply
    - Structurized the software, added serial output, improved the LED to be similar with Julians design
    - Readings and calculations are based on vcc, which is read automatically
    - Much simpler differential control algorithm
    - Schematic better structurized, easier to understand
    - Minor component value changes
    - Eagle schematic and board file available
    - Gerber files available (allows you to order your own boards easily)
*/

const float codeVersion = 1.0; // Software revision

//
// =======================================================================================================
// BUILD OPTIONS (comment out unneeded options)
// =======================================================================================================
//

#define DEBUG // if not commented out, Serial.print() is active! For debugging only!!

//
// =======================================================================================================
// INCLUDE LIRBARIES
// =======================================================================================================
//

// Tabs (header files in sketch directory)
#include "readVCC.h"

// Libraries
#include <statusLED.h> // TheDIYGuy999 library: https://github.com/TheDIYGuy999/statusLED

//
// =======================================================================================================
// PIN ASSIGNMENTS & GLOBAL VARIABLES
// =======================================================================================================
//

// Pins
#define PWM 9
#define PUMP1 3
#define PUMP2 11
#define VSENSE_OUT A1

// Status LED objects
statusLED LED(false); // false = not inversed (LED wired between GND and output)

float outputVoltage;
float vcc = 3.3; // Init value only. Will be read automatically later on. Base for all our voltage readings!
float pwm; // float required for finer granularity during calculation in differential equations!

// Configuration variables (you may have to change them)
float targetOutputVoltage = 13.8; // Our maximum battery charge voltage
float voltageCalibration = 94.7; // (100k + 10k) / 10k = 11  ->  1024 / 11 = 93.09 in theory

//
// =======================================================================================================
// MAIN ARDUINO SETUP (1x during startup)
// =======================================================================================================
//

void setup() {

  // Serial port setup
#ifdef DEBUG
  Serial.begin(19200);
#endif

  // Output setup
  pinMode(PWM, OUTPUT);
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);

  // PWM setup
  TCCR2A = TCCR2A | B00110000; // Inverting mode for charge pump
  TCCR2B = TCCR2B & B11111000 | B001; // Charge pump pins 15kHz
  TCCR1B = TCCR1B & B11111000 | B010; // PWM 1.95kHz (the maximum of this MOSFET driver circuit!)

  analogWrite(PUMP2, 117); // 117
  analogWrite(PUMP1, 137); // 137

  LED.begin(13); // Onboard LED on pin 13
}

//
// =======================================================================================================
// LED
// =======================================================================================================
//

void led() {
  if (pwm == 255) {
    // Indicate battery voltage: 13 flashes = 13V etc.
    LED.flash(20, 380, 700, outputVoltage); // ON, OFF, PAUSE, PULSES
  }

  // Indicate pwm
  else LED.flash((pwm / 2), 128 - (pwm / 2), 0, 0); // ON, OFF, PAUSE, PULSES
}

//
// =======================================================================================================
// CHECK VCC VOLTAGE
// =======================================================================================================
//

void checkVcc(boolean immediately) {

  static unsigned long lastVcc;
  if (millis() - lastVcc >= 200 || immediately) { // Every 200ms or immediately
    lastVcc = millis();
    vcc = readVcc() / 1000.0;
  }
}

//
// =======================================================================================================
// READ SENSORS
// =======================================================================================================
//

void readSensors() {
  outputVoltage = analogRead(VSENSE_OUT) * vcc / voltageCalibration;
}

//
// =======================================================================================================
// CONTROL LOOP
// =======================================================================================================
//

void controlLoop() {
  pwm += targetOutputVoltage - outputVoltage; // simple p (differential) controller
  pwm = constrain(pwm, 0, 255 );
  analogWrite(PWM, pwm);
}

//
// =======================================================================================================
// SERIAL PRINT
// =======================================================================================================
//
void serialPrint() {
#ifdef DEBUG
  static unsigned long lastPrint;
  if (millis() - lastPrint >= 1000) { // Every 1000ms
    lastPrint = millis();

    Serial.print("Setpoint: ");
    Serial.print(targetOutputVoltage);

    Serial.print(" Battery: ");
    Serial.print(outputVoltage);

    Serial.print(" PWM: ");
    Serial.print(pwm, 0);

    Serial.print(" vcc: ");
    Serial.println(vcc);
  }
#endif
}

//
// =======================================================================================================
// MAIN LOOP
// =======================================================================================================
//

void loop() {

  checkVcc(false); // with delay
  readSensors();
  controlLoop();
  led();
  serialPrint();
}

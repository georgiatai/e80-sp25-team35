/* DUMMY CLASS! USE AS A TEMPLATE FOR NEW LIBRARIES! */

// this includes everything from the header file
#include "SensorHall.h"

// this allows you to use print calls
// (see IMU or ADC libraries for good examples)
#include "Printer.h"
extern Printer printer;

// constructor for class objects
SensorHall::SensorHall(void) {
}

void SensorHall::init(const float threshold_in) {
  pinMode(HALL_PIN,INPUT); 
  threshold = threshold_in
  // this lets you print messages
  // the string gets displayed in the messages area
  // the number counts the number of times it is displayed at 10 Hz
  // printer.printMessage("Initialized LED at " + String(millis()), 10);
}

void SensorHall::read(void) {
  int value = analogRead(HALL_PIN);

  voltage = value * (3.3 / 1023.0);

  if (voltage < threshold) {
    // the hall sensor approaches 0V when magnet near, 3V when no magnet
    high = true;
  }
  else {
    high = false;
  }
}

String SensorHall::printVoltage(void) {
  String printString = "Voltage:";

  printString += String(voltage);
  printString += "[V], ";

  return printString;
}
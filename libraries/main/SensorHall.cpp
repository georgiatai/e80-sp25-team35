/* DUMMY CLASS! USE AS A TEMPLATE FOR NEW LIBRARIES! */

// this includes everything from the header file
#include "SensorHall.h"

// this allows you to use print calls
// (see IMU or ADC libraries for good examples)
#include "Printer.h"
extern Printer printer;

// constructor for class objects
SensorHall::SensorHall(void)
  : DataSource("Hall","bool") // from DataSource
{}

void SensorHall::init(const float threshold_in) {
  pinMode(HALL_PIN,INPUT); 
  threshold = threshold_in;
  // this lets you print messages
  // the string gets displayed in the messages area
  // the number counts the number of times it is displayed at 10 Hz
  // printer.printMessage("Initialized LED at " + String(millis()), 10);
}

void SensorHall::read(void) {
  int value = analogRead(HALL_PIN);

  voltage = value * (3.3 / 1023.0);

  if (voltage < threshold) {
    high = true;
  }
  else {
    high = false;
  }
}

size_t SensorHall::writeDataBytes(unsigned char * buffer, size_t idx)
// This function writes data to the micro SD card
{
  bool * data_slot = (bool *) &buffer[idx];
  data_slot[0] = high;
  return idx + sizeof(bool);
}

String SensorHall::printVoltage(void) {
  String printString = "Hall Effect Voltage:";

  printString += String(voltage);
  printString += "[V], ";
  printString += String(high);

  return printString;
}

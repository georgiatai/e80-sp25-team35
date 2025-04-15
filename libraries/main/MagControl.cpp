/* DUMMY CLASS! USE AS A TEMPLATE FOR NEW LIBRARIES! */

// this includes everything from the header file
#include "MagControl.h"

// this allows you to use print calls
// (see IMU or ADC libraries for good examples)
#include "Printer.h"
extern Printer printer;

// constructor for class objects
MagControl::MagControl(void) {
}

void MagControl::init(void) {
  pinMode(MAG_PIN,OUTPUT); // MAG_PIN is defined in pinouts

  // this lets you print messages
  // the string gets displayed in the messages area
  // the number counts the number of times it is displayed at 10 Hz
  // printer.printMessage("Initialized MAG at " + String(millis()), 10);
}

void MagControl::enableMag() {
    digitalWrite(MAG_PIN,HIGH);
}
void MagControl::disableMag() {
    digitalWrite(MAG_PIN,LOW);
}


/* DUMMY CLASS! USE AS A TEMPLATE FOR NEW LIBRARIES! */

// this includes everything from the header file
#include "WinchControl.h"

// this allows you to use print calls
// (see IMU or ADC libraries for good examples)
#include "Printer.h"
extern Printer printer;

// constructor for class objects
WinchControl::WinchControl(void) {
}

void WinchControl::init(void) {
  pinMode(WINCH_EN_PIN,OUTPUT); // GPS_LOCK_LED is defined in pinouts

  // this lets you print messages
  // the string gets displayed in the messages area
  // the number counts the number of times it is displayed at 10 Hz
  // printer.printMessage("Initialized LED at " + String(millis()), 10);
}

void WinchControl::idle(void) {
  mag = true;
  idle = true;
  down = true;
}

void WinchControl::run(float hallVal_in, int currentTime_in, int delayStartTime_in) {
  lastHall = currentHall;
  currentHall = hallVal_in;
  currentTime = currentTime_in;

  if (idle = true) {
    start(delayStartTime_in);
  }


}

void WinchControl::start(int delayStartTime_in) {
  delayStartTime = delayStartTime_in
  mag = false;
}

void WinchControl::countHall(void) {
  if (hallChange()) {
    hallCount++;
  }
}
void WinchControl::lower(void) {
  mag = false;

}

void WinchControl::raise(void) {
  digitalWrite(WINCH_EN_PIN,HIGH);
  mag = false;
}

void WinchControl::stop(void) {
  digitalWrite(WINCH_EN_PIN,LOW);
  mag = true;
}



bool WinchControl::hallChange(void) {
  if (currentHall && !lastHall) {
    return true;
  }
  else {
    return false;
  }
}
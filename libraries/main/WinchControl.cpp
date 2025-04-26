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
  last_hall = false;
  current_hall = false;
  start_time = millis();
  current_time = start_time;
  last_hall_time = start_time;
  idle();


  // this lets you print messages
  // the string gets displayed in the messages area
  // the number counts the number of times it is displayed at 10 Hz
  // printer.printMessage("Initialized LED at " + String(millis()), 10);
}

void WinchControl::idle(void) {
  last_hall_time = current_time;
  state = 0;
  mag = true;
  motor = false;
}
void WinchControl::lower(void) {
  last_hall_time = current_time;
  state = 1;
  mag = false;
  motor = false;
}
void WinchControl::raise(void) {
  last_hall_time = current_time;
  state = 2;
  mag = true;
  motor = true;
}

void WinchControl::run(bool hallVal_in, int currentTime_in) {
  last_hall = current_hall;
  current_hall = hallVal_in;
  current_time = currentTime_in;

  if (current_hall != last_hall) {
    last_hall_time = current_time;
  }
  bool rotation_stopped = (current_time - last_hall_time) > 2000;
  
  if (state == 1 && rotation_stopped) {
    raise();
  }
  else if (state == 2 && rotation_stopped) {
    idle();
  }
}


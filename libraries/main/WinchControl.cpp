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

void WinchControl::idling(void) {
  motor = false;
  mag = true;
  idle = true;
  down = true;
}

void WinchControl::run(bool hallVal_in, int currentTime_in, int delayStartTime_in) {
  lastHall = currentHall;
  currentHall = hallVal_in;
  currentTime = currentTime_in;



  if (idle == true) {
    start(delayStartTime_in);
  }

  if ((currentTime - totalTime) < 5000) {
    down = true;
  } else if ((currentTime - totalTime) < 10000) {
    raise();
  } else {
    stop();
  }
  /*
  if (down == true) {
    countHall(down);
    stopDetector();
  }
  else {
    if (hallCount > 0) {
      raise();
    }
    else{
      stop();
    }
  }
  */
}

void WinchControl::start(int delayStartTime_in) {
  delayStartTime = delayStartTime_in;
  totalTime = currentTime;
  mag = false;
  idle = false;
}

void WinchControl::countHall(bool mode) {
  if (mode && hallChange()) {
    hallCount++;
    lastTime = currentTime;
  }
  else if (!mode && hallChange()) {
    hallCount--;
  }
}

void WinchControl::raise(void) {
  motor = true;
  mag = true;
}

void WinchControl::stop(void) {
  motor = false;
  mag = true; // turn on the magnet
}

void WinchControl::stopDetector(void) {
  int avg = (currentTime-totalTime)/hallCount;
  int delta = currentTime-lastTime;
  if (delta >= avg) {
    down = false;
  }
}

bool WinchControl::hallChange(void) {
  if (currentHall && !lastHall) {
    return true;
  }
  else {
    return false;
  }
}

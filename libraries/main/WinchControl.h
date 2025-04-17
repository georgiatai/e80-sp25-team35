#ifndef __WinchControl_H__
#define __WinchControl_H__

// include relevant libraries, always good to have these two
#include <Arduino.h>
#include "Pinouts.h"
#include "MagControl.h"

// controls how often and when in the loop this class's functions run

class WinchControl {
public: // for functions outside code might call
  WinchControl(void);

  void init(void);

  void run(bool hallVal_in, int currentTime_in, int delayStartTime_in)

  bool mag = true; // should the magnet be on? yes/no
  int lastExecutionTime = -1;

private: // for helper functions and hidden variables

  int hallCount = 0;
  int delayStartTime = 0;
  int currentTime = 0;
  int totalTime = 0;
  int lastTime = 0;
  bool currentHall = false;
  bool lastHall = false;
  bool idle = true;
  bool down = true;

  void countHall(void);

  void start(int delayStartTime_in);
  void countHall(bool mode);
  void lower(void);
  void stopDetector(void);
  void raise(void);
  void stop(void);
  void idle(void);
  
  bool hallChange(void); // returns true if the hall effect sensor voltage has dropped
};

#endif

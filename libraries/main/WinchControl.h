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
  void run(bool hallVal_in, int currentTime_in);

  void idle(void);
  void lower(void);
  void raise(void);

  bool mag;
  bool motor;
  int state;

  int lastExecutionTime = -1;
  int last_hall_time; // make private

private: // for helper functions and hidden variables
  bool last_hall;
  bool current_hall;
  int start_time;
  int current_time;
};

#endif

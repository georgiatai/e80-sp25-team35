/* DUMMY CLASS! USE AS A TEMPLATE FOR NEW LIBRARIES! */

// only define this class if it hasn't been before
#ifndef __MagControl_H__
#define __MagControl_H__

// include relevant libraries, always good to have these two
#include <Arduino.h>
#include "Pinouts.h"

// controls how often and when in the loop this class's functions run


class MagControl {
public: // for functions outside code might call
  MagControl(void);

  void init(void);

  void drive(bool en);


  int lastExecutionTime = -1;

private: // for helper functions and hidden variables
};

#endif

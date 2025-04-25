/* DUMMY CLASS! USE AS A TEMPLATE FOR NEW LIBRARIES! */

// only define this class if it hasn't been before
#ifndef __SensorHall_H__
#define __SensorHall_H__

// include relevant libraries, always good to have these two
#include <Arduino.h>
#include "Pinouts.h"
#include <SensorGPS.h>

// controls how often and when in the loop this class's functions run


class SensorHall : public DataSource {
public: // for functions outside code might call
  SensorHall(void);
  String printVoltage(void);

  void init(const float threshold_in);

  void read(void);
  size_t writeDataBytes(unsigned char * buffer, size_t idx);

  int lastExecutionTime = -1;

  //this is the variable you want
  bool high = false;

private: // for helper functions and hidden variables
  float voltage = 0;
  float threshold = 0;;
};

#endif

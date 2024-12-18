#ifndef __TIMESTAMPER_H__
#define __TIMESTAMPER_H__

#include <Arduino.h>
#include "DataSource.h"
#include "Pinouts.h"


class TimeStamper : public DataSource
{
public:
  TimeStamper(void);

  void init(void);

  // Managing state
  unsigned long curr_time;
  void updateState(void);
  String printState(void);

  // Write out
  size_t writeDataBytes(unsigned char * buffer, size_t idx);

  int lastExecutionTime = -1;
  
};

#endif

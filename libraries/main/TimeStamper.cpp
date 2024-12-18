#include "TimeStamper.h"
#include "Printer.h"

extern Printer printer;

TimeStamper::TimeStamper(void) 
  : DataSource("Time","ulong") // from DataSource
{}

void TimeStamper::updateState(void)
// This function is called in the main loop of Default_Robot.ino
{
  // when the voltage at USER_BUTTON is low, the button 
  // has been pressed, thus buttonState is set to high
  curr_time = micros();
}


String TimeStamper::printState(void)
// This function returns a string that the Printer class 
// can print to the serial monitor if desired
{
  return "Time: " + String(curr_time);
}

size_t TimeStamper::writeDataBytes(unsigned char * buffer, size_t idx)
// This function writes data to the micro SD card
{
  unsigned long * data_slot = (unsigned long *) &buffer[idx];
  data_slot[0] = curr_time;
  return idx + sizeof(unsigned long);
}

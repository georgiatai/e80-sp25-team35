/********
E80 Lab 7 Surface Activity Code
Authors:
    Omar Aleman (oaleman@g.hmc.edu) '21 (contributed 2019)
    Wilson Ives (wives@g.hmc.edu) '20 (contributed in 2018)
    Christopher McElroy (cmcelroy@g.hmc.edu) '19 (contributed in 2017)  
    Josephine Wong (jowong@hmc.edu) '18 (contributed in 2016)
    Apoorva Sharma (asharma@hmc.edu) '17 (contributed in 2016)                    
*/

#include <Arduino.h>
#include <Wire.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <Pinouts.h>
#include <TimingOffsets.h>
#include <SensorGPS.h>
#include <SensorIMU.h>
#include <XYStateEstimator.h>
#include <ADCSampler.h>
#include <ErrorFlagSampler.h>
#include <ButtonSampler.h> // A template of a data source library
#include <MotorDriver.h>
#include <Logger.h>
#include <Printer.h>
#include <SurfaceControl.h>
#define UartSerial Serial1
#include <GPSLockLED.h>
#include <WinchControl.h>
#include <SensorHall.h>
#include <MagControl.h>

/////////////////////////* Global Variables *////////////////////////

MotorDriver motor_driver;
XYStateEstimator xy_state_estimator;
SurfaceControl surface_control;
SensorGPS gps;
Adafruit_GPS GPS(&UartSerial);
ADCSampler adc;
ErrorFlagSampler ef;
ButtonSampler button_sampler;
SensorIMU imu;
Logger logger;
Printer printer;
GPSLockLED led;
WinchControl winch_control;
SensorHall hall;
MagControl mag_driver;

// loop start recorder
int loopStartTime;
int currentTime;
volatile bool EF_States[NUM_FLAGS] = {1,1,1};

////////////////////////* Setup *////////////////////////////////

void setup() {
  
  logger.include(&imu);
  logger.include(&gps);
  logger.include(&xy_state_estimator);
  logger.include(&surface_control);
  logger.include(&motor_driver);
  logger.include(&adc);
  logger.include(&ef);
  logger.include(&button_sampler);
  logger.include(&hall);
  logger.init();

  printer.init();
  ef.init();
  button_sampler.init();
  imu.init();
  UartSerial.begin(9600);
  gps.init(&GPS);
  motor_driver.init();
  led.init();
  // TODO: write init functions
  adc.init();
  winch_control.init();
  mag_driver.init();

  const float hallThreshold = 2.0;
  hall.init(hallThreshold);

  const int num_surface_waypoints = 3; // Number of ordered pairs of surface waypoints. 
  // (e.g., if surface_waypoints is {x0,y0,x1,y1} then num_surface_waypoints is 2.) 
  // Set to 0 if only doing depth control 
  double surface_waypoints [] = { 0, 0 };   // listed as x0,y0,x1,y1, ... etc.
  surface_control.init(num_surface_waypoints, surface_waypoints, 0);
  
  xy_state_estimator.init(); 

  printer.printMessage("Starting main loop",10);
  loopStartTime = millis();
  printer.lastExecutionTime            = loopStartTime - LOOP_PERIOD + PRINTER_LOOP_OFFSET ;
  imu.lastExecutionTime                = loopStartTime - LOOP_PERIOD + IMU_LOOP_OFFSET;
  adc.lastExecutionTime                = loopStartTime - LOOP_PERIOD + ADC_LOOP_OFFSET;
  ef.lastExecutionTime                 = loopStartTime - LOOP_PERIOD + ERROR_FLAG_LOOP_OFFSET;
  button_sampler.lastExecutionTime     = loopStartTime - LOOP_PERIOD + BUTTON_LOOP_OFFSET;
  xy_state_estimator.lastExecutionTime = loopStartTime - LOOP_PERIOD + XY_STATE_ESTIMATOR_LOOP_OFFSET;
  surface_control.lastExecutionTime    = loopStartTime - LOOP_PERIOD + SURFACE_CONTROL_LOOP_OFFSET;
  logger.lastExecutionTime             = loopStartTime - LOOP_PERIOD + LOGGER_LOOP_OFFSET;
  winch_control.lastExecutionTime      = loopStartTime - LOOP_PERIOD + WINCH_CONTROL_LOOP_OFFSET;
  hall.lastExecutionTime               = loopStartTime - LOOP_PERIOD + HALL_LOOP_OFFSET;
}



//////////////////////////////* Loop */////////////////////////

void loop() {
  currentTime=millis();
    
  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    printer.printValue(0,adc.printSample());
    printer.printValue(1,ef.printStates());
    printer.printValue(2,logger.printState());
    printer.printValue(3,gps.printState());   
    printer.printValue(4,xy_state_estimator.printState());  
    printer.printValue(5,surface_control.printWaypointUpdate());
    printer.printValue(6,surface_control.printString());
    printer.printValue(7,motor_driver.printState());
    printer.printValue(8,imu.printRollPitchHeading());        
    printer.printValue(9,imu.printAccels());
    printer.printValue(10,hall.printVoltage());
    printer.printValue(11,
      "delay: " + String(surface_control.delay)
      + "\nmotor: " + String(winch_control.motor)
      + "\tmag: " + String(winch_control.mag)
      + "\nstate: " + String(winch_control.state));
    printer.printToSerial();  // To stop printing, just comment this line out
  }

  /// SURFACE CONTROL FINITE STATE MACHINE///
  if ( currentTime-surface_control.lastExecutionTime > LOOP_PERIOD ) {
    surface_control.lastExecutionTime = currentTime;
    if ( surface_control.navigateState ) { // NAVIGATE STATE //
      if ( !surface_control.atPoint ) { 
        surface_control.navigate(&xy_state_estimator.state, &gps.state, winch_control.state);
      }
      else if ( surface_control.complete ) { 
        delete[] surface_control.wayPoints; // destroy surface waypoint array from the Heap
      }
      else {
        if (winch_control.state == 0) {
          winch_control.lower();
        }
        surface_control.atPoint = false;   // get ready to go to the next point
      }
      motor_driver.drive(surface_control.uL,surface_control.uR,0);
    }
  }

  if ( currentTime-winch_control.lastExecutionTime > LOOP_PERIOD ) {
    winch_control.lastExecutionTime = currentTime;
    
    winch_control.run(hall.high, currentTime);
    // if the bot is sitting waiting at a waypoint, run winch code
    mag_driver.drive(winch_control.mag);
    motor_driver.drive_one(2, -MOTOR_POWER * winch_control.motor);
  }

  if ( currentTime-adc.lastExecutionTime > LOOP_PERIOD ) {
    adc.lastExecutionTime = currentTime;
    adc.updateSample(); 
  }
  
  if ( currentTime-adc.lastExecutionTime > LOOP_PERIOD ) {
    adc.lastExecutionTime = currentTime;
    adc.updateSample(); 
  }

  if ( currentTime-ef.lastExecutionTime > LOOP_PERIOD ) {
    ef.lastExecutionTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(ERROR_FLAG_A), EFA_Detected, LOW);
    attachInterrupt(digitalPinToInterrupt(ERROR_FLAG_B), EFB_Detected, LOW);
    attachInterrupt(digitalPinToInterrupt(ERROR_FLAG_C), EFC_Detected, LOW);
    delay(5);
    detachInterrupt(digitalPinToInterrupt(ERROR_FLAG_A));
    detachInterrupt(digitalPinToInterrupt(ERROR_FLAG_B));
    detachInterrupt(digitalPinToInterrupt(ERROR_FLAG_C));
    ef.updateStates(EF_States[0],EF_States[1],EF_States[2]);
    EF_States[0] = 1;
    EF_States[1] = 1;
    EF_States[2] = 1;
  }

  // uses the ButtonSampler library to read a button -- use this as a template for new libraries!
  if ( currentTime-button_sampler.lastExecutionTime > LOOP_PERIOD ) {
    button_sampler.lastExecutionTime = currentTime;
    button_sampler.updateState();
  }

  if ( currentTime-imu.lastExecutionTime > LOOP_PERIOD ) {
    imu.lastExecutionTime = currentTime;
    imu.read();     // blocking I2C calls
  }
 
  gps.read(&GPS); // blocking UART calls, need to check for UART data every cycle

  if ( currentTime-xy_state_estimator.lastExecutionTime > LOOP_PERIOD ) {
    xy_state_estimator.lastExecutionTime = currentTime;
    xy_state_estimator.updateState(&imu.state, &gps.state);
  }
  
  if ( currentTime-led.lastExecutionTime > LOOP_PERIOD ) {
    led.lastExecutionTime = currentTime;
    led.flashLED(&gps.state);
  }

  if ( currentTime-hall.lastExecutionTime > LOOP_PERIOD ) {
    hall.lastExecutionTime = currentTime;
    hall.read();
  }

  if ( currentTime- logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging ) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }
}

void EFA_Detected(void){
  EF_States[0] = 0;
}

void EFB_Detected(void){
  EF_States[1] = 0;
}

void EFC_Detected(void){
  EF_States[2] = 0;
}














//test
void loop_test() {
  currentTime=millis();
    
  if ( currentTime-printer.lastExecutionTime > LOOP_PERIOD ) {
    printer.lastExecutionTime = currentTime;
    printer.printValue(0,adc.printSample());
    printer.printValue(1,ef.printStates());
    printer.printValue(2,logger.printState());
    printer.printValue(3,gps.printState());   
    printer.printValue(4,xy_state_estimator.printState());  
    printer.printValue(5,surface_control.printWaypointUpdate());
    printer.printValue(6,surface_control.printString());
    printer.printValue(7,motor_driver.printState());
    printer.printValue(8,imu.printRollPitchHeading());        
    printer.printValue(9,imu.printAccels());
    printer.printValue(10,hall.printVoltage());
    printer.printValue(11, "currentTime: " + String(currentTime));
    printer.printValue(12,
        "delay: " + String(surface_control.delay)
        + "\nmotor: " + String(winch_control.motor)
        + "\tmag: " + String(winch_control.mag)
        + "\nstate: " + String(winch_control.state)
        + "\nlast_hall_time: " + String(winch_control.last_hall_time)
        + "\ncurrent_time: " + String(currentTime)
      );
    printer.printToSerial();  // To stop printing, just comment this line out
  }

  if ( currentTime-winch_control.lastExecutionTime > LOOP_PERIOD ) {
    winch_control.lastExecutionTime = currentTime;
    
    winch_control.run(hall.high, currentTime);
    // if the bot is sitting waiting at a waypoint, run winch code
    if ( surface_control.delay && winch_control.state == 0) {
      // do winchy things while the bot isn't moving
      winch_control.lower();
      // maybe have the magnet on, maybe not.
    }
    mag_driver.drive(winch_control.mag);
    motor_driver.drive(0, 0, -MOTOR_POWER * winch_control.motor);
  }

  if ( currentTime-adc.lastExecutionTime > LOOP_PERIOD ) {
    adc.lastExecutionTime = currentTime;
    adc.updateSample(); 
  }

  if ( currentTime-ef.lastExecutionTime > LOOP_PERIOD ) {
    ef.lastExecutionTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(ERROR_FLAG_A), EFA_Detected, LOW);
    attachInterrupt(digitalPinToInterrupt(ERROR_FLAG_B), EFB_Detected, LOW);
    attachInterrupt(digitalPinToInterrupt(ERROR_FLAG_C), EFC_Detected, LOW);
    delay(5);
    detachInterrupt(digitalPinToInterrupt(ERROR_FLAG_A));
    detachInterrupt(digitalPinToInterrupt(ERROR_FLAG_B));
    detachInterrupt(digitalPinToInterrupt(ERROR_FLAG_C));
    ef.updateStates(EF_States[0],EF_States[1],EF_States[2]);
    EF_States[0] = 1;
    EF_States[1] = 1;
    EF_States[2] = 1;
  }

  // uses the ButtonSampler library to read a button -- use this as a template for new libraries!
  if ( currentTime-button_sampler.lastExecutionTime > LOOP_PERIOD ) {
    button_sampler.lastExecutionTime = currentTime;
    button_sampler.updateState();
  }

  if ( currentTime-imu.lastExecutionTime > LOOP_PERIOD ) {
    imu.lastExecutionTime = currentTime;
    imu.read();     // blocking I2C calls
  }
 
  gps.read(&GPS); // blocking UART calls, need to check for UART data every cycle

  if ( currentTime-xy_state_estimator.lastExecutionTime > LOOP_PERIOD ) {
    xy_state_estimator.lastExecutionTime = currentTime;
    xy_state_estimator.updateState(&imu.state, &gps.state);
  }
  
  if ( currentTime-led.lastExecutionTime > LOOP_PERIOD ) {
    led.lastExecutionTime = currentTime;
    led.flashLED(&gps.state);
  }

  if ( currentTime-hall.lastExecutionTime > LOOP_PERIOD ) {
    hall.lastExecutionTime = currentTime;
    hall.read();
  }

  if ( currentTime- logger.lastExecutionTime > LOOP_PERIOD && logger.keepLogging ) {
    logger.lastExecutionTime = currentTime;
    logger.log();
  }
}

.POSIX:

# Compiler
CC =arduino-cli compile
TBOARD =-b teensy:avr:teensy40
LIBS =--libraries libraries

# Flags
CFLAGS =
DFLAGS =


Default_Robot: 
	$(CC) $(TBOARD) $(LIBS) $(@)/$(@).ino

E80_Lab_01: 
	$(CC) $(TBOARD) $(LIBS) $(@)/$(@).ino

E80_Lab_05_Teensy_Rig: 
	$(CC) $(TBOARD) $(LIBS) $(@)/$(@).ino

E80_Lab_07_dive: 
	$(CC) $(TBOARD) $(LIBS) $(@)/$(@).ino

E80_Lab_07_surface: 
	$(CC) $(TBOARD) $(LIBS) $(@)/$(@).ino

Mag_Calibration: 
	$(CC) $(TBOARD) $(LIBS) $(@)/$(@).ino

.PHONY: Default_Robot clean

#ifndef __sample_hardware__
#define __sample_hardware__
enum ELEC350_ERROR_CODE {OK, FATAL};

extern DigitalOut onBoardLED;
extern DigitalOut redLED;
extern DigitalOut yellowLED;
extern DigitalOut greenLED;

extern DigitalIn  onBoardSwitch;
extern DigitalIn  SW1;
extern DigitalIn  SW2;
//extern Serial pc;
extern AnalogIn adcIn;

extern void post();
extern void errorCode(ELEC350_ERROR_CODE err);

#endif
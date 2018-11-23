#include "mbed.h"
#include "sample_hardware.hpp"

#define RED_DONE 1
#define YELLOW_DONE 2

//Digital outputs
DigitalOut onBoardLED(LED1);
DigitalOut redLED(PE_15);
DigitalOut yellowLED(PB_10);
DigitalOut greenLED(PB_11);

//Inputs
DigitalIn  onBoardSwitch(USER_BUTTON);
DigitalIn  SW1(PE_12);
DigitalIn  SW2(PE_14);
//Serial pc(USBTX, USBRX);
AnalogIn adcIn(PA_0);


//POWER ON SELF TEST
void post() 
{
    //POWER ON TEST (POT)
    puts("ALL LEDs should be blinking");
    for (unsigned int n=0; n<10; n++) {
        redLED    = 1;
        yellowLED = 1;
        greenLED  = 1;
        wait(0.05);
        redLED    = 0;
        yellowLED = 0;
        greenLED  = 0;     
        wait(0.05);         
    } 
    
    //Output the switch states (hold them down to test)
    printf("SW1: %d\tSW2: %d\n\r", SW1.read(), SW2.read());    
    
    //Output the ADC
    printf("ADC: %f\n\r", adcIn.read());
}

void errorCode(ELEC350_ERROR_CODE err)
{
            switch (err) {
              case OK:
                greenLED = 1;
                wait(1.0);
                greenLED = 0;
                return;                
              case FATAL:
                while(1) {
                    redLED = 1;
                    wait(0.1);
                    redLED = 0;
                    wait(0.1);                
                }
            };
}
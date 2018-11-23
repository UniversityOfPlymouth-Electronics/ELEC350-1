#include "mbed.h"
#include "sample_hardware.h"
#include "Networkbits.h"

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

//Environmental Sensor driver
#ifdef BME
BME280 sensor(D14, D15);
#else
BMP280 sensor(D14, D15);
#endif

//LCD Driver (provided via mbed repository)
//RS D9
//E  D8
//D7,6,4,2 are the 4 bit for d4-7
TextLCD lcd(D9, D8, D7, D6, D4, D2); // rs, e, d4-d7

//SD Card
SDBlockDevice sd(PB_5, D12, D13, D10); // mosi, miso, sclk, cs 

//POWER ON SELF TEST
void post() 
{
    //POWER ON TEST (POT)
    puts("**********STARTING POWER ON SELF TEST (POST)**********");
    
    //Test LEDs
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
    printf("USER: %d\n\r", onBoardSwitch.read()); 
    
    //Output the ADC
    printf("ADC: %f\n\r", adcIn.read());
    
    //Read Sensors (I2C)
    float temp = sensor.getTemperature();
    float pressure = sensor.getPressure();
    #ifdef BME
    float humidity = sensor.getHumidity();
    #endif
   
    //Display in PuTTY
    printf("Temperature: %5.1f\n", temp);
    printf("Pressure: %5.1f\n", pressure);
    #ifdef BME
    printf("Pressure: %5.1f\n", humidity);
    #endif
    
    //Display on LCD
    redLED = 1;
    lcd.cls();
    lcd.printf("LCD TEST...");
    wait(0.5);
    redLED = 0;
    
    //Network test (if BOTH switches are held down)
    networktest();
    
    puts("**********POST END**********");
 
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

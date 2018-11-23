#include "mbed.h"
//#include "USBSerial.h"
#include "USBMouse.h"
//#include "USBMouseKeyboard.h"
//#include "USBMSD.h"

//DigitalOut myled(LED1);

//USBSerial serial;
//USBMouse mouse;
//USBMouseKeyboard mouse;

USBMouse mouse(ABS_MOUSE);

/*
int main (void)
    {
        while(1){}
}
*/


  int main(void)
  {
    uint16_t x_center = (X_MAX_ABS - X_MIN_ABS)/2;
    uint16_t y_center = ((Y_MAX_ABS - Y_MIN_ABS)/2);
    uint16_t x_screen = 0;
    uint16_t y_screen = 0;
    
    double ratio_x_y = 1920.0 / 1080.0;
   
    printf("Weeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee!\n\r"); 
    //pc.printf("%f\n\r",ratio_x_y);
    
    uint32_t x_origin = x_center;
    uint32_t y_origin = y_center;
    uint32_t radius = 5000;
    uint32_t angle = 0;
 
    while (1)
    {
        x_screen = x_origin + cos((double)angle*3.14/180.0)*radius;
        y_screen = y_origin + sin((double)angle*3.14/180.0)*radius*ratio_x_y;
        mouse.move(x_screen, y_screen+300);
        angle += 3;
        wait(0.01);
    }
  }
  
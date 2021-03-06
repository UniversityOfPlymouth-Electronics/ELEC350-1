/**
 ******************************************************************************
 * @file    main.cpp
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-August-2015
 * @brief   Simple Example application for using the X_NUCLEO_IKS01A1 
 *          MEMS Inertial & Environmental Sensor Nucleo expansion board.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************


 HINTS:
 
 Use a Ticker for accurate sampling, but do NOT use printf or locks inside an ISR. Instead, use a MailBox to safely move data across from an ISR to a Thread
 Many functions in MBED are thread-safe - check the online docs
 
 For buffering, use an Array (of structures) and the producer-consumer pattern (or a variant of it).
 DO NOT use a mailbox or queue to perform the buffering
 
 Perform serial comms on another thread
 
 Beware of a thread running out of stack space. If you have to use a lot of local variable data, consider increasing the size of the stack for the respective thread. See the constructor for Thread in the docs
 
 In terms of diagnostics, consider the following type of information:
 
 An indication that the sampling is running (not every sample maybe, but a heart-beat type indication)
 An error if the buffer is full
 An warning if the buffer is empty
 Anything that helps diagnose a deadlock (e.g. output a message / toggle an LED before a lock is taken and after it is released)
 
 For high marks in the logging aspect, remember that although printf is thread safe (not interrupt safe), printf from multiple threads will result in interleaved text.
 To solve this, have a logging thread that queues up whole messages and write them to the serial interface one at a time - this is ambitious but can be done
*/ 

/* Includes */
#include "mbed.h"
#include "rtos.h"
#include "x_nucleo_iks01a1.h"

/* Instantiate the expansion board */
static X_NUCLEO_IKS01A1 *mems_expansion_board = X_NUCLEO_IKS01A1::Instance(D14, D15);

/* Retrieve the composing elements of the expansion board */
static GyroSensor *gyroscope = mems_expansion_board->GetGyroscope();
static MotionSensor *accelerometer = mems_expansion_board->GetAccelerometer();
static MagneticSensor *magnetometer = mems_expansion_board->magnetometer;

extern char *printDouble(char* str, double v, int decimalDigits=2);

typedef struct  {
    int32_t x;
    int32_t y;
    int32_t z;
} AccelData;

/* Simple main function */
int main() {
  uint8_t id;
  int32_t axes[3];
  
  printf("\r\n--- Starting new run ---\r\n");
  
  accelerometer->ReadID(&id);
  printf("LSM6DS0 Accelerometer             = 0x%X\r\n", id);
  magnetometer->ReadID(&id);
  printf("LIS3MDL magnetometer              = 0x%X\r\n", id);
  gyroscope->ReadID(&id);
  printf("LSM6DS0 accelerometer & gyroscope = 0x%X\r\n", id);
  
  wait(3);
 
  while(1) {
    printf("\r\n");

    magnetometer->Get_M_Axes(axes);
    printf("LIS3MDL [mag/mgauss]:  %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

    accelerometer->Get_X_Axes(axes);
    
    AccelData d;
    d.x = axes[0];
    d.y = axes[1];
    d.z = axes[2];
    
    printf("LSM6DS0 [acc/mg]:      %6ld, %6ld, %6ld\r\n", d.x, d.y, d.z);
    
    gyroscope->Get_G_Axes(axes);
    printf("LSM6DS0 [gyro/mdps]:   %6ld, %6ld, %6ld\r\n", axes[0], axes[1], axes[2]);

    //This is not a good way to do accurate sampling - I reccomend a Ticker
    wait(1.5);
  }
}

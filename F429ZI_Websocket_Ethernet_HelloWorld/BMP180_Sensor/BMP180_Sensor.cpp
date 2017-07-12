/*
BMP180_Sensor.cpp
Used in Library file with BMP180_Sensor.h to enable the following functions

void BMP180_Initialise(void);   //Get calibration data from The BMP180 Sensor
int Read_Temperature(void);     //Reads the UnCalibrated Temperature Data from the BMP180
int Read_Pressure(void);        //Reads the Uncalibrated Pressure Data from the BMP180
int trueTemp(int);              //Corrects the data to a True Temperature Using the Devices Calibration Information
int truePress(int);             //Corrects the data to a True Pressure Using the Devices Calibration Information

*/
#include "mbed.h"
#include "BMP180_Sensor.h"

short AC1,AC2,AC3,B1,B2,MB,MC,MD,oss=0;
unsigned short AC4,AC5,AC6;
long B3,B5,B6,X1,X2,X3,p;
unsigned long B4,B7;
char ucdata_write[2],ucdata_read[2];
long True_Temp,True_Press;
int siUTemp=0;
long siUPress=0;


//I2C i2c(PF_0,PF_1);//I2C_SDA, I2C_SCL);       //I2C Class Pin Assignments see I2C.h
I2C i2c(I2C_SDA, I2C_SCL);       //I2C Class Pin Assignments see I2C.h

int trueTemp(int siUT)
{ // Convert to True Temperature from BMP180 Datasheet
    //True_Temp=0;
    X1=((siUT-AC6)*AC5)/(1<<15);
    X2=(MC*(1<<11))/(X1+MD);
    B5=X1+X2;
    True_Temp=(B5+8)/(1<<4);
    return True_Temp;
}

int truePress(int siUP)
{ // Convert to True Pressure from BMP180 Datasheet
    oss=0;p=0;
    B6=B5-4000;
    X1=(B2*(B6*B6)/(1<<12))/(1<<11);
    X2=(AC2*B6)/(1<<11);
    X3=X1+X2;
    B3=(((AC1*4+X3)<<oss)+2)/4;
    X1=AC3*B6/(1<<13);
    X2=(B1*(B6*B6/(1<<12)))/(1<<16);
    X3=((X1+X2)+2)/(1<<2);
    B4=AC4*(unsigned long)(X3+32768)/(1<<15);
    B7=((unsigned long)siUP-B3)*(50000>>oss);
    if (B7<0x80000000){p=(B7*2)/B4;}
    else {p=(B7/B4)*2;}
    X1=(p/(1<<8))*(p/(1<<8));
    X1=(X1*3038)/(1<<16);
    X2=(-7357*p)/(1<<16);
    p=p+(X1+X2+3791)/(1<<4);                //(where p is the TRUE Calibrated Pressure in Pa)
    return p;
}
signed int readBMP(int addy)
{
    short int siReadData;
    ucdata_write[0] = addy;                                 //Address here to read
    while (i2c.write(BMP180_ADDR, ucdata_write, 1, 0)){}    //Wait for ACK if DEVICE is in 'write' cycle
    i2c.read(BMP180_ADDR,ucdata_read,2,0);                  //Note 'OR' Address with Read bit Done in Library
    siReadData=ucdata_read[0]<<8;
    siReadData=siReadData+ucdata_read[1];
    return siReadData;
 }
 
 void BMP180_Initialise(void)
 {//Get Calibration Data from BMP180 Device stored in it's Registers
        AC1=readBMP(AC1_MSB);
        AC2=readBMP(AC2_MSB);
        AC3=readBMP(AC3_MSB);
        AC4=readBMP(AC4_MSB);
        AC5=readBMP(AC5_MSB);
        AC6=readBMP(AC6_MSB);
        B1=readBMP(B1_MSB);
        B2=readBMP(B2_MSB);
        MB=readBMP(MB_MSB);
        MC=readBMP(MC_MSB);
        MD=readBMP(MD_MSB);
}
int Read_Temperature (void)
{
            ucdata_write[0]=0xF4;                               //Control Register
            ucdata_write[1]=0x2E;                               //Control Code to Start Temperature Read
            i2c.write(BMP180_ADDR, ucdata_write,2,0);           //Send Code to Device
            wait_ms(5);                                         //WAIT FOR CONVERSION TO COMPLETE (See Pressure setting for Notes on This Delay)
            ucdata_write[0]=0xF6;                               //Result Register,(NB Can be used for Temperature OR Pressure depending on
                                                                //what was asked for previously!
            i2c.write(BMP180_ADDR, ucdata_write,1,0);           //Write to the i2c device with the register address
            i2c.read(BMP180_ADDR,ucdata_read,2,0);              //Read the data from the i2c device at the register address as above
            siUTemp=ucdata_read[0]*0x100+ucdata_read[1];        //Put both Bytes into a 16bit Signed Word
            return siUTemp;
}    
int Read_Pressure (void)
{
            ucdata_write[0]=0xF4;                               //Control Register
            ucdata_write[1]=0x34;                               //Control Code to Start Pressure Read
            i2c.write(BMP180_ADDR, ucdata_write,2,0);           //Send Code to Device
            wait_ms(5);                                         //WAIT FOR CONVERSION TO COMPLETE
                                                                //NB to self don't like this should be checking for ACK from device to i2c Master.
                                                                //However checking Datasheet you HAVE to use a delay.
                                                                //Since we have selected sampling rate of 1
                                                                //a 4.5ms Max time required for Pressure Data to settle.
                                                                //So use a delay of 5ms (next highest integer).
                                                                //For the Temperature data the time to settle is ALWAYS a Maximum of 4.5ms
                                                                //So use a delay of 5ms (next highest integer).
            ucdata_write[0]=0xF6;                               //Result Register,(NB Can be used for Temperature OR Pressure depending on
                                                                //what was asked for previously!
            i2c.write(BMP180_ADDR, ucdata_write,1,0);           //Write to the i2c device with the register address
            i2c.read(BMP180_ADDR, ucdata_read,2,0);             //Read the data from the i2c device at the register address as above
 

            siUPress=ucdata_read[0]*0x100+ucdata_read[1];       //Put both Bytes into a 16bit Signed Word
            return siUPress;
 }
int Temperature(void)
{
    siUTemp=Read_Temperature();
    True_Temp=trueTemp(siUTemp);
    return True_Temp;
}
int Pressure(void)
{
    siUPress=Read_Pressure();
    True_Press=truePress(siUPress);
    return True_Press;
}

    

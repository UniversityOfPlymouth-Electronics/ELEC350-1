#ifndef BMP180_SENSOR_H
#define BMP180_SENSOR_H

#define BMP180_ADDR        (0xEE) // BMP180 Pressure/Temperature Sensor using i2c Address
#define WRITE              (0x00) // BMP180 using i2c Write bit
#define READ               (0x01) // BMP180 using i2c Read bit
#define BMP180_MID         (0xD0) // Manufacturer ID Address (Read Only 0x55==Bosch)
#define AC1_MSB            (0xAA) // Calibration registers to get true Temperature and Pressure
#define AC1_LSB            (0xAB) // Calibration
#define AC2_MSB            (0xAC) // Calibration
#define AC2_LSB            (0xAD) // Calibration
#define AC3_MSB            (0xAE) // Calibration
#define AC3_LSB            (0xAF) // Calibration
#define AC4_MSB            (0xB0) // Calibration
#define AC4_LSB            (0xB1) // Calibration
#define AC5_MSB            (0xB2) // Calibration
#define AC5_LSB            (0xB3) // Calibration
#define AC6_MSB            (0xB4) // Calibration
#define AC6_LSB            (0xB5) // Calibration
#define B1_MSB             (0xB6) // Calibration
#define B1_LSB             (0xB7) // Calibration
#define B2_MSB             (0xB8) // Calibration
#define B2_LSB             (0xB9) // Calibration
#define MB_MSB             (0xBA) // Calibration
#define MB_LSB             (0xBB) // Calibration
#define MC_MSB             (0xBC) // Calibration
#define MC_LSB             (0xBD) // Calibration
#define MD_MSB             (0xBE) // Calibration
#define MD_LSB             (0xBF) // End of Calibration Registers
#define OUT_MSB            (0xF6) // Result of EITHER Temp or Pressure depending on what was asked for MSB
#define OUT_LSB            (0xF7) // Result of EITHER Temp or Pressure depending on what was asked for LSB
#define OUT_XLSB           (0xF8) // Result of EITHER Temp or Pressure depending on what was asked for XLSB!
#define SOFT_RST           (0xE0) // Soft Reset if you wish to restart the sensor

extern short AC1,AC2,AC3,B1,B2,MB,MC,MD,oss;
extern unsigned short AC4,AC5,AC6;
extern long B3,B5,B6,X1,X2,X3,p;
extern unsigned long B4,B7;
extern char ucdata_write[2],ucdata_read[2];
extern long True_Temp,True_Press;
extern int siUTemp;
extern long siUPress;
extern short int siReadData;

void BMP180_Initialise(void);
int Read_Temperature(void);
int Read_Pressure(void);
int trueTemp(int);
int truePress(int);
int Temperature(void);
int Pressure(void);
#endif

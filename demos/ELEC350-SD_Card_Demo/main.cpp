// NickO 2017
/* Access an SD Card using SPI */
 
 #include "mbed.h"
 #include "SDBlockDevice.h"
 #include "FATFileSystem.h"
 #include "sample_hardware.hpp"
 
 //SD Card Object
 SDBlockDevice sd(PB_5, D12, D13, D10); // mosi, miso, sclk, cs

 uint8_t block[512] = "Hello World!\n";
 int main()
{
    //POWER ON SELF TEST
    post();
    
    printf("Initialise\n");
    //FileSystemLike(*sd);

    // call the SDBlockDevice instance initialisation method.
    if ( sd.init() != 0) {
        printf("Init failed \n");
        errorCode(FATAL);
    }    
    
    //Create a filing system for SD Card
    FATFileSystem fs("sd", &sd);
    
    // *************
    // Open to WRITE
    // *************
    printf("Write to a file\n");
    FILE* fp = fopen("/sd/test.txt","w");
    //Check file handle (stream)
    if (fp == NULL) {
        error("Could not open file for write\n");
        errorCode(FATAL);
    }
    
    //Put some text in the file...
    fprintf(fp, "Welcome to ELEC350\n");
    
    //Close the file
    fclose(fp);
    
    // ************
    // Open to READ
    // ************
    printf("Read a file\n");
    fp = fopen("/sd/test.txt","r");
    if (fp == NULL) {
        error("Could not open file for read\n");
        errorCode(FATAL);
    }   
    
    //Read in three strings
    char s1[64], s2[64], s3[64];
    fscanf(fp, "%s %s %s", s1,s2,s3);
    //To read a whole line, use: fgets(s1, sizeof(s1), fp);
    printf("READ BACK: %s %s %s\n", s1, s2, s3);
    
    //Close File
    fclose(fp);
    
    //Close down
    sd.deinit();
    printf("All done...\n");
    errorCode(OK);
    
    //Flash to indicate goodness
    while(true) {
        greenLED = 1;
        wait(0.5);
        greenLED = 0;
        wait(0.1);    
    }
}

   /* 
    printf("sd size: %llu\n",         sd.size());
    printf("sd read size: %llu\n",    sd.get_read_size());
    printf("sd program size: %llu\n", sd.get_program_size());
    printf("sd erase size: %llu\n",   sd.get_erase_size());

    // set the frequency
    if ( 0 != sd.frequency(5000000)) {
        printf("Error setting frequency \n");
    }

    if ( 0 != sd.erase(0, sd.get_erase_size())) {
        printf("Error Erasing block \n");
    }

    // Write some the data block to the device
    if ( 0 == sd.program(block, 0, 512)) {
        // read the data block from the device
        if ( 0 == sd.read(block, 0, 512)) {
            // print the contents of the block
            printf("%s", block);
        }
    }

    // call the SDBlockDevice instance de-initialisation method.
    
    sd.deinit();
    */
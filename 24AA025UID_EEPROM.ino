/*****************************************************
 *  24AA025UID Read Write Test
 *  File:   24AA025UID_EEPROM.ino
 *  Date:   2016_04_28
 *  Author: Stephen Minakian
 *  Version:0.1.0
 *
 *  Description:
 *    Simple demonstration app for reading and
 *    writing to the 24AA025UID EEPROM, including
 *    Reading the device UID.
 *****************************************************/

#include "24AA025UID_EEPROM.h"

EEPROM1 myEEPROM;

void setup() {
    Serial1.begin(115200);
    Serial1.println("Started");
    myEEPROM.begin();
    uint8_t data[6];

    //Wire.begin();
    //Wire.beginTransmission(0x50);
    //Serial1.println(String(Wire.write(0xFA), HEX));
    //Serial1.println(String(Wire.endTransmission(false), HEX));
    //Wire.requestFrom(0x50, 6);

    uint8_t pos = 0;

    //for(pos = 0; pos < 6; pos++){
    //    Serial1.println(String(Wire.read(), HEX));
    //}

    myEEPROM.readData(data, 0xfa, 6); // Reads the UID Manually
    for(pos = 0; pos<6;pos++){
        Serial1.println(String(data[pos], HEX));
    }
    Serial1.println(String(myEEPROM.getUID(), HEX)); // Reads the UID obtained from myEEPROM.begin();
    
    
    uint8_t writeData[100];
    for(pos = 0; pos < 100; pos++){
        writeData[pos] = 100 - pos;
    }
    Serial1.println("\r\nWrite Test");
    myEEPROM.writeData(writeData, 0x04, 100);   // Tests writing large amounts of data across multiple pages
    uint8_t readData[100];
    myEEPROM.readData(readData, 0x04, 100);     // Reads the written data for verification
    for(pos = 0; pos<100;pos++){
        Serial1.println(String(readData[pos], DEC));    // Prints the data to the terminal
    }

}

void loop() {

}
/*****************************************************
 *  24AA025UID Library
 *  File:   24AA025UID_EEPROM.cpp
 *  Date:   2016_04_28
 *  Author: Stephen Minakian
 *  Version:0.1.0
 * 
 * **************************
 * There is a small section of code that blocks for 
 * 5ms after performing a write. This is so that
 * pages may be written or a read may be performed 
 * immediately after the write.
 *****************************************************/
/*****************************************************
 *  TODO: Add error handling.
 *****************************************************/



#include "24AA025UID_EEPROM.h"

// Write pages are different for the 24AA02UID and 24AA25UID

/* Public Functions */

EEPROM1::EEPROM1(){
    wireAddress = 0x50;
}

//EEPROM::EEPROM(uint8_t address) {
//    wireAddress = address;
//    #ifdef _DEBUG
//        Serial1.begin(115200);
//        Serial1.println("EEPROM address = " + String(wireAddress, HEX));
//    #endif
//}
void EEPROM1::begin(void){
    if(!Wire.isEnabled()){
        Wire.begin();
        //Serial1.println("Wire started");
    }
    UID = readUID();
}

uint8_t EEPROM1::readData(uint8_t * dataBuffer, uint8_t startAddress, uint8_t bytes){
    readConsecutive(dataBuffer, startAddress, bytes>32?32:bytes);
    if(bytes > 32){
        readData(dataBuffer+32, startAddress+32, bytes-32);
    }
    
//    if(bytes > 1){
//        readConsecutive(dataBuffer, startAddress, bytes);
//    } else if(bytes == 1){
//        *dataBuffer = readByte(startAddress);
//    }
}

uint8_t EEPROM1::writeData(uint8_t * dataBuffer, uint8_t startAddress, uint8_t bytes){
    // Upto 8 bytes can be written at a time, if the data is greater than 8 bytes, divide into 8 byte or smaller chunks
    
//    if(bytes == 1){
//        //writeByte(startAddress, *dataBuffer);
//    } else if(bytes > 8){
    uint8_t bytesOfPageRemaining = 0;
    bytesOfPageRemaining = 0x20 - (startAddress%0x20);
    bytesOfPageRemaining = bytesOfPageRemaining%16;
    if((bytes > bytesOfPageRemaining) &&(bytesOfPageRemaining != 0)){
        bytesOfPageRemaining = bytesOfPageRemaining%16;
        writePage(dataBuffer, startAddress, bytesOfPageRemaining);
        dataBuffer += bytesOfPageRemaining;
        startAddress += bytesOfPageRemaining;
        bytes -= bytesOfPageRemaining;
    }
    writePage(dataBuffer, startAddress, bytes>MAX_WRITE_PAGE?MAX_WRITE_PAGE:bytes);
    if(bytes > MAX_WRITE_PAGE){
        writeData(dataBuffer + MAX_WRITE_PAGE, startAddress + MAX_WRITE_PAGE, bytes - MAX_WRITE_PAGE);
    } else {
        return 0;
    }
}

uint32_t EEPROM1::getUID(void){
    return UID;
}

/* Private Functions */

uint8_t EEPROM1::readByte(uint8_t dataAddress){
    Wire.beginTransmission(wireAddress);
    Wire.write(dataAddress);
    if(Wire.endTransmission(false)){
        // Handle error
        return 0; // May need value here <------
    }
    if(!Wire.requestFrom(wireAddress, 1)){
        // Handle error
        return 0; // May need value here <------
    }
    return Wire.read();
}

uint8_t EEPROM1::readConsecutive(uint8_t * dataBuffer, uint8_t startAddress, uint8_t bytes){
    Wire.beginTransmission(wireAddress);
    Wire.write(startAddress);
    Wire.endTransmission(false);
    if(Wire.requestFrom(wireAddress, bytes) != bytes){
        return 0;
    } else {
        uint8_t pos;
        for(pos = 0; pos < bytes; pos++){
            dataBuffer[pos] = Wire.read();
        }
        return bytes;
    }
}

uint8_t EEPROM1::writeByte(uint8_t dataAddress, uint8_t data){
    Wire.beginTransmission(wireAddress);
    Wire.write(dataAddress);
    Wire.write(data);
    return Wire.endTransmission();
}

uint8_t EEPROM1::writePage(uint8_t * dataBuffer, uint8_t startAddress, uint8_t bytes){
    //Serial1.println("Writing " + String(bytes, DEC) + " to address " + String(startAddress, HEX));
    Wire.beginTransmission(wireAddress);
    Wire.write(startAddress);
    Wire.write(dataBuffer, bytes);
    uint8_t status = Wire.endTransmission();
    delay(5);   // This is the only intentional piece of blocking code  **********
    return status;
}

uint32_t EEPROM1::readUID(void){
    uint8_t UIDBytes[4];
    if(readConsecutive(UIDBytes, 0xFC, 4) != 4){
        return 0;
    }
    uint8_t pos;
    UID = 0;        // Probably not needed
    for(pos = 0; pos < 4; pos++){
        UID <<= 8;
        UID |= UIDBytes[pos];
    }
    return UID;
}
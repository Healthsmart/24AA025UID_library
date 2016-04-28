/*****************************************************
 *  24AA025UID Library
 *  File:   24AA025UID_EEPROM.h
 *  Date:   2016_04_28
 *  Author: Stephen Minakian
 *  Version:0.1.0
 *****************************************************/

#ifndef _24AA025UID_H
#define _24AA025UID_H

#include "application.h"

#define EEPROM_25AA025UID
//#define EEPROM_25AA02UID

#if defined (EEPROM_25AA025UID) && defined (EEPROM_25AA02UID)
    #error "Cannot have both parts defined"
#endif
#ifdef EEPROM_25AA025UID
    #define MAX_WRITE_PAGE 16
#endif
#ifdef EEPROM_25AA02UID
    #define MAX_WRITE_PAGE 8
#endif

class EEPROM1 {
    public:
        // Functions
        EEPROM1();
        //EEPROM(uint8_t address);
        void begin(void);
        uint8_t readData(uint8_t * dataBuffer, uint8_t startAddress, uint8_t bytes);
        uint8_t writeData(uint8_t * dataBuffer, uint8_t startAddress, uint8_t bytes);
        uint32_t getUID(void);

    private:
        // Variables
        uint8_t wireAddress = 0x50;
        uint8_t position;
        uint32_t UID;
        
        // Functions
        
        uint8_t readByte(uint8_t dataAddress);
        uint8_t readConsecutive(uint8_t * dataBuffer, uint8_t startAddress, uint8_t bytes);
        uint8_t writeByte(uint8_t dataAddress, uint8_t data);
        uint8_t writePage(uint8_t * dataBuffer, uint8_t startAddress, uint8_t bytes);

        uint32_t readUID(void);
};

#endif
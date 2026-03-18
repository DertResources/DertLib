#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "../include/DataEntry.h"
#include "../include/SerializeHelper.h"
#include "../include/PortabilityHelper.h"

namespace dlibsim
{

typedef enum : Byte{
    CAN, // 0 
    PWM, // 1
    DIO  // 2
} IdType;

class Device
{
private:
    std::string deviceName;                 // Standard
    IdType idType;                          // Standard
    uint8_t idNumber;                       // Unique
    std::string idString;                   // Unique
    std::vector<DataEntry> dataEntryVector; // Standard
    uint8_t DataEntryCount;                 // Standard
public:
    Device();
    inline Device& SetDeviceName(std::string __deviceName)       { deviceName = __deviceName; return *this; }
    inline Device& SetIdString(std::string __idString)           { idString =     __idString; return *this; }
    inline Device& SetIdType(IdType __idType)                    { idType =         __idType; return *this; }
    inline Device& SetIdNumber(uint8_t __idNumber)               { idNumber =     __idNumber; return *this; }
    inline Device& AddDataEntry(DataEntry &&obj)                 {dataEntryVector.push_back(std::move(obj)); 
                                                                  DataEntryCount++; 
                                                                  return *this; }
    void WriteToBuffer(ByteVector& byteBuffer);
    void ReadFromBuffer(const Byte* begin, const Byte*& next);
    void Print(uint8_t tabs = 0);
};

}
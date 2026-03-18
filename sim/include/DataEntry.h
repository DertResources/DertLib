#pragma once
#include <cstdint>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include "../include/SerializeHelper.h"
#include "../include/PortabilityHelper.h"

namespace dlibsim
{

typedef enum : Byte {
    BOOL, // 0
    INT,  // 1
    FLOAT // 2
} DataType;

typedef enum : Byte {
    DESIREDSTATE, // 0
    FEEDBACK      // 1
} DataDirection;

class DataEntry {
private:
    std::string name;
    uint8_t sizeInBytes;
    bool isSigned;
    DataDirection direction;
    DataType dataType;
    Byte DataDescription;
    uint16_t byteOffset;
public:
    DataEntry();
    static DataEntry GetDefault() {DataEntry obj; return obj;}
    inline DataEntry& SetName(std::string __name)           {name        = __name        ; return *this;}
    inline DataEntry& SetSizeInBytes(uint8_t __sizeInBytes) {sizeInBytes = __sizeInBytes ; return *this;}
    inline DataEntry& SetIsSigned(bool __isSigned)          {isSigned    = __isSigned    ; return *this;}
    inline DataEntry& SetDataType(DataType __dataType)      {dataType    = __dataType    ; return *this;}
    inline DataEntry& SetDataDirection(DataDirection __direction){ direction= __direction; return *this;}
    inline DataEntry& SetByteOffset(uint16_t __byteOffset) { byteOffset  = __byteOffset ; return *this;}
    void WriteToBuffer(ByteVector& byteBuffer);
    void ReadFromBuffer(const Byte* begin, const Byte*& next);
    bool operator==(const DataEntry &other) const;
    void Print(uint8_t tabs = 0);
};

};

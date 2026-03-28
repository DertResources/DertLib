#include "../include/DataEntry.h"

namespace dlibsim
{

DataEntry::DataEntry()
: name{""}
, sizeInBytes{0}
, isSigned{true}
, direction{DataDirection::DESIREDSTATE}
, dataType{DataType::FLOAT}
, DataDescription{0}
, byteOffset{0}
{}

void DataEntry::WriteToBuffer(ByteVector& byteBuffer)
{
    write_string(byteBuffer, name);

    DataDescription = static_cast<Byte>(direction)    << 7|
                      static_cast<Byte>(isSigned)    << 6 |
                      static_cast<Byte>(dataType)    << 4 |
                      std::clamp<Byte>(sizeInBytes, 0, 8 );
    write_u8_be(byteBuffer, DataDescription);
    write_u16_be(byteBuffer, byteOffset);
}

void DataEntry::ReadFromBuffer(const Byte* begin, const Byte*& next)
{
    const Byte* link1 = nullptr;
    const Byte* link2 = nullptr;
    name = read_string(begin, link1);

    DataDescription = read_u8_be(link1, link2);
    direction = DataDirection(static_cast<uint32_t>(DataDescription >> 7));
    isSigned = (DataDescription & 64) >> 6;
    dataType = DataType((DataDescription & 48) >> 4);
    sizeInBytes = DataDescription & 15;

    byteOffset = read_u16_be(link2, next);
}

bool DataEntry::operator==(const DataEntry& other) const
{
    if(name.compare(other.name))
        return false;
    return (DataDescription == other.DataDescription);
}

void DataEntry::Print(uint8_t tabs)
{
    std::string tabString = std::string(2*tabs, ' ');
    // Name
    dlib::print_t(tabString + "\033[92mData Entry: \"" + name + "\""  + "\n");
    // Is Signed
    dlib::print_t(tabString + "  is signed: " + (isSigned ? "true" : "false") + "\n");
    // Data Type
    dlib::print_t(tabString + "  data type: ");
    switch (dataType)
    {
    case DataType::BOOL:
        dlib::print_t("boolean");
        break;
    case DataType::FLOAT:
        dlib::print_t("floating-point");
        break; 
    case DataType::INT:
        dlib::print_t("integer");
        break;
    default:
        dlib::print_t("unknown");
        break;
    }
    dlib::print_t("\n");
    //Data Direction
    dlib::print_t(tabString + "  data direction: ");
    switch (direction)
    {
    case DataDirection::DESIREDSTATE:
        dlib::print_t("desired state");
        break;
    case DataDirection::FEEDBACK:
        dlib::print_t("sensor feedback");
        break;
    default:
        dlib::print_t("unknown");
        break;
    }
    dlib::print_t("\n");
    // Size in Bytes
    dlib::print_t(tabString + "  size in bytes: " + std::to_string(static_cast<uint32_t>(sizeInBytes)) + "\n");
    dlib::print_t(tabString + "  byte offset: " + std::to_string(static_cast<uint32_t>(byteOffset)) + "\033[m\n");
}

};
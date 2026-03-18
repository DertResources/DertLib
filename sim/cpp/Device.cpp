#include "../include/Device.h"

namespace dlibsim
{

Device::Device()
: deviceName{"device name"}
, idType{IdType::CAN}
, idNumber{0}
, idString{"id string"}
, dataEntryVector{}
, DataEntryCount{0}
{}

void Device::WriteToBuffer(ByteVector& byteBuffer)
{
    write_u8_be(byteBuffer, DataEntryCount);
    write_string(byteBuffer, deviceName);
    write_u8_be(byteBuffer, idType);
    write_u8_be(byteBuffer, idNumber);
    write_string(byteBuffer, idString);
    for (auto entry : dataEntryVector)
        entry.WriteToBuffer(byteBuffer);
}

void Device::ReadFromBuffer(const Byte* begin, const Byte*& next)
{
    const Byte* link1;
    const Byte* link2;
    DataEntryCount = read_u8_be(begin, link1);
    deviceName = read_string(link1, link2);
    idType = IdType(static_cast<uint32_t>(read_u8_be(link2, link1)));
    idNumber = read_u8_be(link1, link2);
    idString = read_string(link2, link1);
    dataEntryVector.clear();
    for (size_t pos = 0; pos < DataEntryCount; pos++)
    {
        dataEntryVector.push_back(DataEntry::GetDefault());
        dataEntryVector.at(pos).ReadFromBuffer(link1, link2);
        link1 = link2;
    }
}

void Device::Print(uint8_t tabs)
{
    std::string tabString = std::string(tabs*2, ' ');
    dlib::print_t( tabString + "\033[91mDevice Name: \"" + deviceName + "\"\n");
    dlib::print_t( tabString + "  id type: ");
    switch (idType)
    {
    case IdType::CAN:
        dlib::print_t("CAN\n");
        dlib::print_t(tabString + "  CAN id: " + std::to_string(idNumber) + "\n");
        dlib::print_t(tabString + "  CANBus name: \"" + idString + "\"" + "\n");
        break;
    case IdType::DIO:
        dlib::print_t("Digital In-Out\n");
        dlib::print_t(tabString + "  DIO port: " + std::to_string(idNumber) + "\n");
        break;
    case IdType::PWM:
        dlib::print_t("Pulse Width Modulation\n");
        dlib::print_t(tabString + "  PWM port: " + std::to_string(idNumber) + "\n");
        break;
    default:
        dlib::print_t("unknown");
        break;
    }
    dlib::print_t( tabString + "[\033[m\n");
    for (auto entry : dataEntryVector) {
        entry.Print(tabs + 1);
    }
    dlib::print_t(tabString + "\033[91m]\033[m\n");
    
}

};
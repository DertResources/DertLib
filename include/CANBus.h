#pragma once
// Std
#include <string>

// Ctre
#include <ctre/phoenix6/CANBus.hpp>

namespace dlib
{

class CANBus
{
public:
    CANBus(std::string _IN_CANBusName);

    operator ctre::phoenix6::CANBus() const;
    operator std::string() const;

private:
    std::string CANBusName;
    ctre::phoenix6::CANBus CTRE_CanBus;
};

};
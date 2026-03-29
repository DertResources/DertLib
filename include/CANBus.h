#pragma once
// Std
#include <string>
#include <memory>

// Ctre
#include <ctre/phoenix6/CANBus.hpp>

namespace dlib
{

class CANBus
{
public:
    CANBus(std::string _IN_CANBusName);
    CANBus(const CANBus &obj);

    operator ctre::phoenix6::CANBus&() const;

private:
    std::string CANBusName;
    std::shared_ptr<ctre::phoenix6::CANBus> CTRE_CanBus;
};

};
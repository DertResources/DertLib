    #include "../include/CANBus.h"
    
    dlib::CANBus::CANBus(std::string _IN_CANBusName)
    : CTRE_CanBus{std::make_shared<ctre::phoenix6::CANBus>(_IN_CANBusName)}
    {}
    
    dlib::CANBus::operator ctre::phoenix6::CANBus() const
    {
        return *CTRE_CanBus;
    }

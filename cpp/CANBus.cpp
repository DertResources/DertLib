    #include "../include/CANBus.h"
    
    dlib::CANBus::CANBus(std::string _IN_CANBusName)
    : CANBusName{_IN_CANBusName}
    , CTRE_CanBus{_IN_CANBusName}
    {}
    
    dlib::CANBus::operator ctre::phoenix6:CANBus() const
    {
        return CTRE_CanBus;
    }

    dlib::CANBus::operator std::string() const
    {
        return CANBusName;
    }

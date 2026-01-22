    #include "../include/CANBus.h"
    
    dlib::CANBus::CANBus(std::string _IN_CANBusName)
    : CTRE_CanBus{std::make_shared<ctre::phoenix6::CANBus>(_IN_CANBusName)}
    , CANBusName{_IN_CANBusName}
    {}
    
    dlib::CANBus::CANBus(const dlib::CANBus &obj)
    : CTRE_CanBus{obj.CTRE_CanBus}
    , CANBusName{obj.CANBusName}
    {}

    dlib::CANBus::operator ctre::phoenix6::CANBus&() const;
    {
        return *CTRE_CanBus;
    }

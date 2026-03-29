    #include "../include/CANBus.h"
    
    dlib::CANBus::CANBus(std::string _IN_CANBusName)
    : CANBusName{std::move(_IN_CANBusName)}
    , CTRE_CanBus{std::make_shared<ctre::phoenix6::CANBus>(_IN_CANBusName)}
    {}
    
    dlib::CANBus::CANBus(const dlib::CANBus &obj)
    : CANBusName{obj.CANBusName}
    , CTRE_CanBus{obj.CTRE_CanBus}
    {}

    dlib::CANBus::operator ctre::phoenix6::CANBus&() const
    {
        return *CTRE_CanBus;
    }

#include "../include/KrakenSet.h"

dlib::KrakenSet::KrakenSet(std::initializer_list<KrakenTalonCreateInfo> createInfos)
{
    for(auto createInfo : createInfos)
    {
        motorSet.push_back(std::make_unique<KrakenTalon>(createInfo));
        addCallbacks(*motorSet.back());
    }
}

void dlib::KrakenSet::stop()
{
    for(auto& motor : motorSet)
    {
        motor->stopMotor();
    }
}

void dlib::KrakenSet::set(double dutyCycle)
{
    for(auto& motor : motorSet)
    {
        motor->setDutyCycle(dutyCycle);
    }
}

void dlib::KrakenSet::setBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
    {
        motor->setBrakeMode(isBrakeMode);
    }
}

void dlib::KrakenSet::addCallbacks(KrakenTalon& motor)
{
    if(motor.finalCreateInfo.setDutyCycleCallback != nullptr)
        DutyCycleCallbacks.push_back(std::bind(&KrakenTalon::setDutyCycleCallback, &motor));
    if(motor.finalCreateInfo.getPositionCallback != nullptr)
        PositionCallbacks.push_back(std::bind(&KrakenTalon::getPositionCallback, &motor));
    if(motor.finalCreateInfo.getVelocityCallback != nullptr)
        VelocityCallbacks.push_back(std::bind(&KrakenTalon::getVelocityCallback, &motor));
}

void dlib::KrakenSet::pushData()
{
    pushVelocities();
    pushPositions();
}

void dlib::KrakenSet::pullCommands()
{
    for(auto func : DutyCycleCallbacks)
        func();
}

void dlib::KrakenSet::pushVelocities()
{
    for(auto func : VelocityCallbacks)
        func();
}

void dlib::KrakenSet::pushPositions()
{
    for(auto func : PositionCallbacks)
        func();
}
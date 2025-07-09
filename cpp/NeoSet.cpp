#include "../include/NeoSet.h"

dlib::NeoSet::NeoSet(std::initializer_list<NeoSparkCreateInfo> createInfos)
{
    for(auto createInfo : createInfos)
    {
        motorSet.push_back(std::make_unique<NeoSpark>(createInfo));
        addCallbacks(*motorSet.back());
    }
}

void dlib::NeoSet::stop()
{
    for(auto& motor : motorSet)
        motor->stopMotor();
}

void dlib::NeoSet::set(double dutyCycle)
{
    for(auto& motor : motorSet)
        motor->setDutyCycle(dutyCycle);
}

void dlib::NeoSet::setBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
        motor->setBrakeModeWhenIdle(isBrakeMode);
}

void dlib::NeoSet::addCallbacks(NeoSpark& motor)
{
    if(motor.finalCreateInfo.setDutyCycleCallback != nullptr)
        pullCommandCalbacks.push_back(std::bind(&NeoSpark::setDutyCycleCallback, &motor));
    if(motor.finalCreateInfo.getPositionCallback != nullptr)
        pushDataCalbacks.push_back(std::bind(&NeoSpark::getPositionCallback, &motor));
    if(motor.finalCreateInfo.getVelocityCallback != nullptr)
        pushDataCalbacks.push_back(std::bind(&NeoSpark::getVelocityCallback, &motor));
}

void dlib::NeoSet::pushData()
{
    for(auto func : pushDataCalbacks)
        func();
}

void dlib::NeoSet::pullCommands()
{
    for(auto func : pullCommandCalbacks)
        func();
}
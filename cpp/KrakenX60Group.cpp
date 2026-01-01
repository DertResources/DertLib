#include "../include/KrakenX60Group.h"

/** Constructor for the kraken Set 
 * @param createInfos array of kraken create infos
 */
dlib::KrakenX60Group::KrakenX60Group(std::initializer_list<KrakenX60MotorCreateInfo> createInfos)
{
    for(auto createInfo : createInfos)
    {
        motorSet.push_back(std::make_unique<KrakenX60Motor>(createInfo));
        AddCallbacks(*motorSet.back());
    }
}

/** Stop all motors */
void dlib::KrakenX60Group::Stop()
{
    for(auto& motor : motorSet)
    {
        motor->StopMotor();
    }
}

/** Set the duty cycle of all motors
 * @param dutyCycle duty cycle for all motors
 */
void dlib::KrakenX60Group::Set(double dutyCycle)
{
    for(auto& motor : motorSet)
    {
        motor->SetDutyCycle(dutyCycle);
    }
}

/** Set the brake mode when idle (coast / break) for all motors 
 * @param isBrakeMode the mode the motors will be in when command is zero.
 * true for break, false for coast
 */
void dlib::KrakenX60Group::SetBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
    {
        motor->setBrakeMode(isBrakeMode);
    }
}

/** Add all callbacks to the callback vectors
 * @param motor reference to KrackenTalon object
 */
void dlib::KrakenX60Group::AddCallbacks(KrakenX60Motor& motor)
{
    if(motor.finalCreateInfo.DutyCycleCallback != nullptr)
        dutyCycleCallbacks.push_back(std::bind(&KrakenX60Motor::DutyCycleCallback, &motor));
    if(motor.finalCreateInfo.PositionCallback != nullptr)
        positionCallbacks.push_back(std::bind(&KrakenX60Motor::PositionCallback, &motor));
    if(motor.finalCreateInfo.VelocityCallback != nullptr)
        velocityCallbacks.push_back(std::bind(&KrakenX60Motor::VelocityCallback, &motor));
}

/** Pull positions and velocities from motor controllers and push them into simulink */
void dlib::KrakenX60Group::SendSensorDataToSL()
{
    SendVelocityValuesToSL();
    SendPositionValuesToSL();
}

/** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
void dlib::KrakenX60Group::FetchMotorCommandsFromSL()
{
    for(auto func : dutyCycleCallbacks)
        func();
}

/** Pull motor angular velocities from motor controllers and push them into simulink */
void dlib::KrakenX60Group::SendVelocityValuesToSL()
{
    for(auto func : velocityCallbacks)
        func();
}

/** Pull motor angular positions from motor controllers and push them into simulink */
void dlib::KrakenX60Group::SendPositionValuesToSL()
{
    for(auto func : positionCallbacks)
        func();
}
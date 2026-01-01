#include "../include/KrakenX60Group.h"

/** Constructor for the kraken Set 
 * @param createInfos Array of kraken create infos
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
 * @param dutyCycle Duty cycle for all motors
 */
void dlib::KrakenX60Group::Set(double dutyCycle)
{
    for(auto& motor : motorSet)
    {
        motor->SetDutyCycle(dutyCycle);
    }
}

/** Set the brake mode when idle (coast / break) for all motors 
 * @param isBrakeMode The mode the motors will be in when command is zero.
 * true for break, false for coast
 */
void dlib::KrakenX60Group::SetBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
    {
        motor->SetBrakeMode(isBrakeMode);
    }
}

/** Add all callbacks to the callback vectors
 * @param motor Reference to KrackenTalon object
 */
void dlib::KrakenX60Group::AddCallbacks(KrakenX60Motor& motor)
{
    if(motor.finalCreateInfo.dutyCycleCallback != nullptr)
        dutyCycleCallbacks.push_back(std::bind(&KrakenX60Motor::FetchDutyCycleFromSLCallback, &motor));
    if(motor.finalCreateInfo.positionCallback != nullptr)
        positionCallbacks.push_back(std::bind(&KrakenX60Motor::SendPositionToSLCallback, &motor));
    if(motor.finalCreateInfo.velocityCallback != nullptr)
        velocityCallbacks.push_back(std::bind(&KrakenX60Motor::SendVelocityToSLCallback, &motor));
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
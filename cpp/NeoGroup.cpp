#include "../include/NeoGroup.h"
/** Constructor for the Neo Set 
 *  @param createInfos array of Neo create infos
 */
dlib::NeoGroup::NeoGroup(std::initializer_list<NeoMotorCreateInfo> createInfos)
{
    for(auto createInfo : createInfos)
    {
        motorSet.push_back(std::make_unique<NeoMotor>(createInfo));
        AddCallbacks(*motorSet.back());
    }
}

/** Stop all motors */
void dlib::NeoGroup::Stop()
{
    for(auto& motor : motorSet)
        motor->StopMotor();
}

/** Set the duty cycle of all motors
 * @param dutyCycle duty cycle for all motors
 */
void dlib::NeoGroup::Set(double dutyCycle)
{
    for(auto& motor : motorSet)
        motor->SetDutyCycle(dutyCycle);
}

/** Set the brake mode when idle (coast / break) for all motors 
 * @param isBrakeMode the mode the motors will be in when command is zero.
 * true for break, false for coast
 */
void dlib::NeoGroup::SetBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
        motor->SetBrakeMode(isBrakeMode);
}

/** Add all callbacks to the callback vectors
 * @param motor reference to NeoMotor object
 */
void dlib::NeoGroup::AddCallbacks(NeoMotor& motor)
{
    if(motor.finalCreateInfo.DutyCycleCallback != nullptr)
        dutyCycleCallbacks.push_back(std::bind(&NeoMotor::FetchDutyCycleFromSLCallback, &motor));
    if(motor.finalCreateInfo.PositionCallback != nullptr)
        positionCallbacks.push_back(std::bind(&NeoMotor::SendPositionToSLCallback, &motor));
    if(motor.finalCreateInfo.VelocityCallback != nullptr)
        velocityCallbacks.push_back(std::bind(&NeoMotor::SendVelocityToSLCallback, &motor));
}



/** Pull positions and velocities from motor controllers and push them into simulink */
void dlib::NeoGroup::SendSensorDataToSL()
{
    SendVelocityValuesToSL();
    SendPositionValuesToSL();
}

/** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
void dlib::NeoGroup::FetchMotorCommandsFromSL()
{
    for(auto func : dutyCycleCallbacks)
        func();
}

/** Pull motor angular velocities from motor controllers and push them into simulink */
void dlib::NeoGroup::SendVelocityValuesToSL()
{
    for(auto func : velocityCallbacks)
        func();
}

/** Pull motor angular positions from motor controllers and push them into simulink */
void dlib::NeoGroup::SendPositionValuesToSL()
{
    for(auto func : positionCallbacks)
        func();
}
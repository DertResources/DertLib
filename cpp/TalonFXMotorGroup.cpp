#include "../include/TalonFXMotorGroup.h"

std::vector<dlib::TalonFXMotorGroup*> dlib::TalonFXMotorGroup::allTalonFXMotorGroups;

/** Constructor for the TalonFX motor group 
 * @param createInfos Array of TalonFX motor create infos
 */
dlib::TalonFXMotorGroup::TalonFXMotorGroup(std::initializer_list<TalonFXMotorCreateInfo> createInfos)
{
    allTalonFXMotorGroups.push_back(this);
    
    for(auto createInfo : createInfos)
    {
        motorSet.push_back(std::make_unique<TalonFXMotor>(createInfo));
        AddCallbacks(*motorSet.back());
    }
}

/** Stop all motors */
void dlib::TalonFXMotorGroup::Stop()
{
    for(auto& motor : motorSet)
    {
        motor->StopMotor();
    }
}

/** Set the duty cycle of all motors
 * @param dutyCycle Duty cycle for all motors
 */
void dlib::TalonFXMotorGroup::Set(double dutyCycle)
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
void dlib::TalonFXMotorGroup::SetBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
    {
        motor->SetBrakeMode(isBrakeMode);
    }
}

/** Add all callbacks to the callback vectors
 * @param motor Reference to KrackenTalon object
 */
void dlib::TalonFXMotorGroup::AddCallbacks(TalonFXMotor& motor)
{
    if(motor.finalCreateInfo.dutyCycleCallback != nullptr)
        dutyCycleCallbacks.push_back(std::bind(&TalonFXMotor::FetchDutyCycleFromSLCallback, &motor));
    if(motor.finalCreateInfo.positionCallback != nullptr)
        positionCallbacks.push_back(std::bind(&TalonFXMotor::SendPositionToSLCallback, &motor));
    if(motor.finalCreateInfo.velocityCallback != nullptr)
        velocityCallbacks.push_back(std::bind(&TalonFXMotor::SendVelocityToSLCallback, &motor));
}

/** Pull positions and velocities from motor controllers and push them into simulink */
void dlib::TalonFXMotorGroup::SendSensorDataToSL()
{
    SendVelocityValuesToSL();
    SendPositionValuesToSL();
}

/** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
void dlib::TalonFXMotorGroup::FetchMotorCommandsFromSL()
{
    for(auto func : dutyCycleCallbacks)
        func();
}

/** Pull motor angular velocities from motor controllers and push them into simulink */
void dlib::TalonFXMotorGroup::SendVelocityValuesToSL()
{
    for(auto func : velocityCallbacks)
        func();
}

/** Pull motor angular positions from motor controllers and push them into simulink */
void dlib::TalonFXMotorGroup::SendPositionValuesToSL()
{
    for(auto func : positionCallbacks)
        func();
}

/** Update the warnings of CAN connections */
void dlib::TalonFXMotorGroup::UpdateMotorCANConnectionAlerts()
{
    for(auto& motor : motorSet)
    {
        motor->UpdateCANConnectionAlert();
    }
}
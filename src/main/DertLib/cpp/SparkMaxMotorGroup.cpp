#include "../include/SparkMaxMotorGroup.h"

std::vector<dlib::SparkMaxMotorGroup*> dlib::SparkMaxMotorGroup::allSparkMaxMotorGroups;

/** Constructor for the SparkMax motor Set 
 *  @param createInfos array of SparkMax motor create infos
 */
dlib::SparkMaxMotorGroup::SparkMaxMotorGroup(std::initializer_list<SparkMaxMotorCreateInfo> createInfos)
{
    allSparkMaxMotorGroups.push_back(this);

    for(auto createInfo : createInfos)
    {
        motorSet.push_back(std::make_unique<SparkMaxMotor>(createInfo));
        AddCallbacks(*motorSet.back());
    }
}

/** Stop all motors */
void dlib::SparkMaxMotorGroup::Stop()
{
    for(auto& motor : motorSet)
        motor->StopMotor();
}

/** Set the duty cycle of all motors
 * @param dutyCycle duty cycle for all motors
 */
void dlib::SparkMaxMotorGroup::Set(double dutyCycle)
{
    for(auto& motor : motorSet)
        motor->SetDutyCycle(dutyCycle);
}

/** Set the brake mode when idle (coast / break) for all motors 
 * @param isBrakeMode the mode the motors will be in when command is zero.
 * true for break, false for coast
 */
void dlib::SparkMaxMotorGroup::SetBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
        motor->SetBrakeMode(isBrakeMode);
}

/** Add all callbacks to the callback vectors
 * @param motor reference to SparkMaxMotor object
 */
void dlib::SparkMaxMotorGroup::AddCallbacks(SparkMaxMotor& motor)
{
    if(motor.finalCreateInfo.dutyCycleCallback != nullptr)
        dutyCycleCallbacks.push_back(std::bind(&SparkMaxMotor::FetchDutyCycleFromSLCallback, &motor));
    if(motor.finalCreateInfo.positionCallback != nullptr)
        positionCallbacks.push_back(std::bind(&SparkMaxMotor::SendPositionToSLCallback, &motor));
    if(motor.finalCreateInfo.velocityCallback != nullptr)
        velocityCallbacks.push_back(std::bind(&SparkMaxMotor::SendVelocityToSLCallback, &motor));
}



/** Pull positions and velocities from motor controllers and push them into simulink */
void dlib::SparkMaxMotorGroup::SendSensorDataToSL()
{
    SendVelocityValuesToSL();
    SendPositionValuesToSL();
}

/** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
void dlib::SparkMaxMotorGroup::FetchMotorCommandsFromSL()
{
    for(auto func : dutyCycleCallbacks)
        func();
}

/** Pull motor angular velocities from motor controllers and push them into simulink */
void dlib::SparkMaxMotorGroup::SendVelocityValuesToSL()
{
    for(auto func : velocityCallbacks)
        func();
}

/** Pull motor angular positions from motor controllers and push them into simulink */
void dlib::SparkMaxMotorGroup::SendPositionValuesToSL()
{
    for(auto func : positionCallbacks)
        func();
}

/** Update the warnings of CAN connections */
void dlib::SparkMaxMotorGroup::UpdateMotorCANConnectionAlerts()
{
    for(auto& motor : motorSet)
    {
        motor->UpdateCANConnectionAlert();
    }
}

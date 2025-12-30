#include "../include/NeoSet.h"
/** Constructor for the Neo set 
 *  @param createInfos array of Neo create infos
 */
dlib::NeoSet::NeoSet(std::initializer_list<NeoSparkCreateInfo> createInfos)
{
    for(auto createInfo : createInfos)
    {
        motorSet.push_back(std::make_unique<NeoSpark>(createInfo));
        addCallbacks(*motorSet.back());
    }
}

/** Stop all motors */
void dlib::NeoSet::stop()
{
    for(auto& motor : motorSet)
        motor->stopMotor();
}

/** Set the duty cycle of all motors
 * @param dutyCycle duty cycle for all motors
 */
void dlib::NeoSet::set(double dutyCycle)
{
    for(auto& motor : motorSet)
        motor->setDutyCycle(dutyCycle);
}

/** Set the brake mode when idle (coast / break) for all motors 
 * @param isBrakeMode the mode the motors will be in when command is zero.
 * true for break, false for coast
 */
void dlib::NeoSet::setBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
        motor->setBrakeModeWhenIdle(isBrakeMode);
}

/** Add all callbacks to the callback vectors
 * @param motor reference to NeoSpark object
 */
void dlib::NeoSet::addCallbacks(NeoSpark& motor)
{
    if(motor.finalCreateInfo.setDutyCycleCallback != nullptr)
        pullCommandCalbacks.push_back(std::bind(&NeoSpark::setDutyCycleCallback, &motor));
    if(motor.finalCreateInfo.getPositionCallback != nullptr)
        pushDataCalbacks.push_back(std::bind(&NeoSpark::getPositionCallback, &motor));
    if(motor.finalCreateInfo.getVelocityCallback != nullptr)
        pushDataCalbacks.push_back(std::bind(&NeoSpark::getVelocityCallback, &motor));
}

/** Pull positions and velocities from motor controllers and push them into simulink */
void pushData()
{
    pushVelocities();
    pushPositions();
}

/** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
void pullCommands()
{
    for(auto func : DutyCycleCallbacks)
        func();
}

/** Pull motor angular velocities from motor controllers and push them into simulink */
void pushVelocities()
{
    for(auto func : VelocityCallbacks)
        func();
}

/** Pull motor angular positions from motor controllers and push them into simulink */
void pushPositions()
{
    for(auto func : PositionCallbacks)
        func();
}
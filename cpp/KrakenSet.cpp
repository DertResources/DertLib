#include "../include/KrakenSet.h"

/** Constructor for the kraken set 
         * @param createInfos array of kraken create infos
        */
dlib::KrakenSet::KrakenSet(std::initializer_list<KrakenTalonCreateInfo> createInfos)
{
    for(auto createInfo : createInfos)
    {
        motorSet.push_back(std::make_unique<KrakenTalon>(createInfo));
        addCallbacks(*motorSet.back());
    }
}

/** Stop all motors */
void dlib::KrakenSet::stop()
{
    for(auto& motor : motorSet)
    {
        motor->stopMotor();
    }
}

/** Set the duty cycle of all motors
 * @param dutyCycle duty cycle for all motors
 */
void dlib::KrakenSet::set(double dutyCycle)
{
    for(auto& motor : motorSet)
    {
        motor->setDutyCycle(dutyCycle);
    }
}

/** Set the brake mode when idle (coast / break) for all motors 
 * @param isBrakeMode the mode the motors will be in when command is zero.
 * true for break, false for coast
 */
void dlib::KrakenSet::setBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
    {
        motor->setBrakeMode(isBrakeMode);
    }
}

/** Add all callbacks to the callback vectors
 * @param motor reference to KrackenTalon object
 */
void dlib::KrakenSet::addCallbacks(KrakenTalon& motor)
{
    if(motor.finalCreateInfo.setDutyCycleCallback != nullptr)
        DutyCycleCallbacks.push_back(std::bind(&KrakenTalon::setDutyCycleCallback, &motor));
    if(motor.finalCreateInfo.getPositionCallback != nullptr)
        PositionCallbacks.push_back(std::bind(&KrakenTalon::getPositionCallback, &motor));
    if(motor.finalCreateInfo.getVelocityCallback != nullptr)
        VelocityCallbacks.push_back(std::bind(&KrakenTalon::getVelocityCallback, &motor));
}

/** Pull positions and velocities from motor controllers and push them into simulink */
void dlib::KrakenSet::pushData()
{
    pushVelocities();
    pushPositions();
}

/** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
void dlib::KrakenSet::pullCommands()
{
    for(auto func : DutyCycleCallbacks)
        func();
}

/** Pull motor angular velocities from motor controllers and push them into simulink */
void dlib::KrakenSet::pushVelocities()
{
    for(auto func : VelocityCallbacks)
        func();
}

/** Pull motor angular positions from motor controllers and push them into simulink */
void dlib::KrakenSet::pushPositions()
{
    for(auto func : PositionCallbacks)
        func();
}
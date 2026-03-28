#include "../../include/TalonFX/TalonFXCreateInfo.h"

namespace dlib
{

namespace TalonFX
{

// // // // // // // // // //
// // //  BASE INFO  // // //
// // // // // // // // // //

/** Modify Create Info@param __IN__canID New CAN ID value @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetCanID(const int& __IN__canID)
{ this->canID = __IN__canID; return *this; }

/** Modify Create Info @param __IN__canbus New canbus value @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetCanbus(const ctre::phoenix6::CANBus& __IN__canbus)
{ this->canbus = __IN__canbus; return *this; }

/** Modify Create Info @param __IN__isReversed New motor reversal value @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetIsReversed(const bool& __IN__isReversed)
{ this->isReversed = __IN__isReversed; return *this; }

/** Modify Create Info @param __IN__supplyCurrentLimit New supply current limit value @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetSupplyCurrentLimit(const double& __IN__supplyCurrentLimit)
{ this->supplyCurrentLimit = __IN__supplyCurrentLimit; return *this; }

/** Modify Create Info @param __IN__openLoopRampPeriod New open loop ramp period value @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetOpenLoopRampPeriod(const double& __IN__openLoopRampPeriod)
{ this->openLoopRampPeriod = __IN__openLoopRampPeriod; return *this; }

/** Modify Create Info @param __IN__enableFOC If true, field oriented control is enabled @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetEnableFOC(const bool& __IN__enableFOC)
{ this->enableFOC = __IN__enableFOC; return *this; }

/** Modify Create Info @param __IN__motorType Type of motor this controller is plugged into @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetMotorType(const MotorType& __IN__motorType)
{ this->motorType = __IN__motorType; return *this; }

/** Modify Create Info @param __IN__velocityCallback New angular velocity callback pointer @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetVelocityCallback(double*& __IN__velocityCallback)
{ this->velocityCallback = __IN__velocityCallback; return *this; }

/** Modify Create Info @param __IN__positionCallback New angular position callback pointer @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetPositionCallback(double*& __IN__positionCallback)
{ this->positionCallback = __IN__positionCallback; return *this; }

/** Modify Create Info @param __IN__displayName New display name for Alerts @return Modified copy of create info*/
BaseMotorCreateInfo& BaseMotorCreateInfo::SetDisplayName(const std::string& __IN__displayName)
{ this->displayName = __IN__displayName; return *this; }

// // // // // // // // // //
// // // DUTY  CYCLE // // //
// // // // // // // // // //

/** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
DutyCycleCreateInfo& DutyCycleCreateInfo::SetDutyCycleCallback(double*& __IN__dutyCycleCallback)
{ this->dutyCycleCallback = __IN__dutyCycleCallback; return *this; }


}; // namespace TalonFX

}; //namespace dlib
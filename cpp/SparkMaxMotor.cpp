#include "../include/SparkMaxMotor.h"

/** constructor for the Kraken Talon */
dlib::SparkMaxMotor::SparkMaxMotor(SparkMaxMotorCreateInfo createInfo)
: sparkMax{createInfo.canID, createInfo.motorType},
  disconectedCANAlert{alertGroupName, createInfo.displayName, frc::Alert::AlertType::kWarning}
{
    rev::spark::SparkMaxConfig config{};
    config
        .Inverted(createInfo.isReversed)
        .SmartCurrentLimit(createInfo.smartCurrentLimit)
        .SecondaryCurrentLimit(createInfo.secondaryCurrentLimit)
        .OpenLoopRampRate(createInfo.openLoopRampRate);
    sparkMax.Configure(config,
                       rev::spark::SparkBase::ResetMode::kResetSafeParameters,
                       rev::spark::SparkBase::PersistMode::kNoPersistParameters);
    if(createInfo.velocityCallback != nullptr || createInfo.positionCallback != nullptr)
        sparkRelEncoder = sparkMax.GetEncoder();
    finalCreateInfo = createInfo;
}

/** Callback for getting position of motor */
void dlib::SparkMaxMotor::SendPositionToSLCallback()
{
    *finalCreateInfo.positionCallback = sparkRelEncoder->GetPosition();
}

/** Callback for getting velocity of motor */
void dlib::SparkMaxMotor::SendVelocityToSLCallback()
{
    *finalCreateInfo.velocityCallback = sparkRelEncoder->GetVelocity();
}

/** Callback for setting duty cycle of motor */
void dlib::SparkMaxMotor::FetchDutyCycleFromSLCallback()
{
    sparkMax.Set(*finalCreateInfo.dutyCycleCallback);
}

/** Set brake mode of motor */
void dlib::SparkMaxMotor::SetBrakeMode(bool isBrakeMode)
{
    rev::spark::SparkMaxConfig config{};
    config.SetIdleMode((isBrakeMode) ?  rev::spark::SparkBaseConfig::IdleMode::kBrake :  rev::spark::SparkBaseConfig::IdleMode::kCoast );
    sparkMax.Configure(config,
                       rev::spark::SparkBase::ResetMode::kNoResetSafeParameters,
                       rev::spark::SparkBase::PersistMode::kNoPersistParameters);
}

/** Set duty cycle of motor */
void dlib::SparkMaxMotor::SetDutyCycle(double DC)
{
    sparkMax.Set(DC);
}

/** Stop motor */
void dlib::SparkMaxMotor::StopMotor()
{
    sparkMax.StopMotor();
}

/** Infers if the motor is connected based on errors */
void dlib::SparkMaxMotor::UpdateCANConnectionAlert()
{
    sparkMax.GetBusVoltage();
    rev::REVLibError lastError = sparkMax.getLastError();
    disconectedCANAlert.Set(lastError != rev::REVLibError::kOK);
}

// Create info functions

dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::getDefaultCreateInfo()
{
    SparkMaxMotorCreateInfo modifiedCreateInfo;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__canID New CAN ID value @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetCanID(int __IN__canID)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.canID = __IN__canID;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__isReversed New motor reversal value @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetIsReversed(bool __IN__isReversed)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.isReversed = __IN__isReversed;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__smartCurrentLimit New smart current limit value @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetSmartCurrentLimit(uint8_t __IN__smartCurrentLimit)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.smartCurrentLimit = __IN__smartCurrentLimit;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__secondaryCurrentLimit New secondary current limit value @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetSecondaryCurrentLimit(double __IN__secondaryCurrentLimit)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.secondaryCurrentLimit = __IN__secondaryCurrentLimit;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__openLoopRampRate New open loop ramp rate value @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetOpenLoopRampRate(double __IN__openLoopRampRate)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.openLoopRampRate = __IN__openLoopRampRate;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetDutyCycleCallback(double* __IN__dutyCycleCallback)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.dutyCycleCallback = __IN__dutyCycleCallback;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__velocityCallback New angular velocity callback pointer @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetVelocityCallback(double* __IN__velocityCallback)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.velocityCallback = __IN__velocityCallback;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__positionCallback New angular position callback pointer @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetPositionCallback(double* __IN__positionCallback)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.positionCallback = __IN__positionCallback;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__motorType New motor type value @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetMotorType(rev::spark::SparkMax::MotorType __IN__motorType)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.motorType = __IN__motorType;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__displayName New name of motor for alerts @return Modified copy of create info*/
dlib::SparkMaxMotorCreateInfo dlib::SparkMaxMotorCreateInfo::SetDisplayName(rev::spark::SparkMax::MotorType __IN__displayName)
{
    SparkMaxMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.displayName = __IN__displayName;
    return modifiedCreateInfo;
}

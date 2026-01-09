#include "../include/SparkMaxMotor.h"

/** constructor for the Kraken Talon */
dlib::SparkMaxMotor::SparkMaxMotor(SparkMaxMotorCreateInfo createInfo)
: sparkMax{createInfo.canID, createInfo.motorType}
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

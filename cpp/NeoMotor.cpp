#include "../include/NeoMotor.h"

/** constructor for the Kraken Talon */
dlib::NeoMotor::NeoMotor(NeoMotorCreateInfo createInfo)
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
    if(createInfo.VelocityCallback != nullptr || createInfo.PositionCallback != nullptr)
        sparkRelEncoder = sparkMax.GetEncoder();
    finalCreateInfo = createInfo;
}

/** Callback for getting position of motor */
void dlib::NeoMotor::SendPositionToSLCallback()
{
    *finalCreateInfo.positionCallback = sparkRelEncoder->GetPosition();
}

/** Callback for getting velocity of motor */
void dlib::NeoMotor::SendVelocityToSLCallback()
{
    *finalCreateInfo.velocityCallback = sparkRelEncoder->GetVelocity();
}

/** Callback for setting duty cycle of motor */
void dlib::NeoMotor::FetchDutyCycleFromSLCallback()
{
    sparkMax.Set(*finalCreateInfo.dutyCycleCallback);
}

/** Set brake mode of motor */
void dlib::NeoMotor::SetBrakeMode(bool isBrakeMode)
{
    rev::spark::SparkMaxConfig config{};
    config.SetIdleMode((isBrakeMode) ?  rev::spark::SparkBaseConfig::IdleMode::kBrake :  rev::spark::SparkBaseConfig::IdleMode::kCoast );
    sparkMax.Configure(config,
                       rev::spark::SparkBase::ResetMode::kNoResetSafeParameters,
                       rev::spark::SparkBase::PersistMode::kNoPersistParameters);
}

/** Set duty cycle of motor */
void dlib::NeoMotor::SetDutyCycle(double DC)
{
    sparkMax.Set(DC);
}

/** Stop motor */
void dlib::NeoMotor::StopMotor()
{
    sparkMax.StopMotor();
}
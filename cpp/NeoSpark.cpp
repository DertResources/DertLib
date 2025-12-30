#include "../include/NeoSpark.h"

/** constructor for the Kraken Talon */
dlib::NeoSpark::NeoSpark(NeoSparkCreateInfo createInfo)
: sparkMax{createInfo.canID, createInfo.motorType}
{
    initalizeSpark(createInfo);
}

/** Not useful now, but was created because there used to be multiple constructors */
void dlib::NeoSpark::initalizeSpark(NeoSparkCreateInfo createInfo)
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
    if(createInfo.getVelocityCallback != nullptr || createInfo.getPositionCallback != nullptr)
        sparkRelEncoder = sparkMax.GetEncoder();
    finalCreateInfo = createInfo;
}

/** Callback for getting position of motor */
void dlib::NeoSpark::getPositionCallback()
{
    *finalCreateInfo.getPositionCallback = sparkRelEncoder->GetPosition();
}

/** Callback for getting velocity of motor */
void dlib::NeoSpark::getVelocityCallback()
{
    *finalCreateInfo.getVelocityCallback = sparkRelEncoder->GetVelocity();
}

/** Callback for setting duty cycle of motor */
void dlib::NeoSpark::setDutyCycleCallback()
{
    sparkMax.Set(*finalCreateInfo.setDutyCycleCallback);
}

/** set brake mode of motor */
void dlib::NeoSpark::setBrakeMode(bool isBrakeMode)
{
    rev::spark::SparkMaxConfig config{};
    config.SetIdleMode((isBrakeMode) ?  rev::spark::SparkBaseConfig::IdleMode::kBrake :  rev::spark::SparkBaseConfig::IdleMode::kCoast );
    sparkMax.Configure(config,
                       rev::spark::SparkBase::ResetMode::kNoResetSafeParameters,
                       rev::spark::SparkBase::PersistMode::kNoPersistParameters);
}

/** set duty cycle of motor */
void dlib::NeoSpark::setDutyCycle(double DC)
{
    sparkMax.Set(DC);
}

/** stop motor */
void dlib::NeoSpark::stopMotor()
{
    sparkMax.StopMotor();
}
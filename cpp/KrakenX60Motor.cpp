#include "../include/KrakenX60Motor.h"

/** constructor for the Kraken Talon */
dlib::KrakenX60Motor::KrakenX60Motor(KrakenX60MotorCreateInfo createInfo)
: talonController{createInfo.canID, createInfo.canbusName},
dutyCycleControl{0}
{
    // Get Configurator to Apply configuration settings
    auto& talonConfigurator = talonController.GetConfigurator();

    // Set Direction
    ctre::phoenix6::configs::MotorOutputConfigs motorDirectionConfig;
    motorDirectionConfig.Inverted = createInfo.isReversed ? 
        ctre::phoenix6::signals::InvertedValue::CounterClockwise_Positive : 
        ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
    talonConfigurator.Apply(motorDirectionConfig);

    // Set Open Loop Ramp Period
    ctre::phoenix6::configs::OpenLoopRampsConfigs rampPeriodConfig;
    units::time::second_t openLoopRampPeriod_time {createInfo.openLoopRampPeriod};
    rampPeriodConfig.DutyCycleOpenLoopRampPeriod = openLoopRampPeriod_time;
    talonConfigurator.Apply(rampPeriodConfig);

    // Set Current Limiting Configuration
    ctre::phoenix6::configs::CurrentLimitsConfigs currentLimitsConfig;
    units::current::ampere_t supplyCurrentLimit_amps {createInfo.supplyCurrentLimit};
    currentLimitsConfig.SupplyCurrentLimit = supplyCurrentLimit_amps;
    currentLimitsConfig.SupplyCurrentLimitEnable = true;
    talonConfigurator.Apply(currentLimitsConfig);

    // Initialize Duty Cycle Output Communication
    dutyCycleControl.WithEnableFOC(createInfo.enableFOC);

    // Store for later use
    finalCreateInfo = createInfo;
}

/** Callback for getting position of motor */
void dlib::KrakenX60Motor::SendPositionToSLCallback()
{  
    *finalCreateInfo.positionCallback = talonController.GetPosition().GetValueAsDouble();
}

/** Callback for getting velocity of motor */
void dlib::KrakenX60Motor::SendVelocityToSLCallback()
{  
    *finalCreateInfo.velocityCallback = talonController.GetVelocity().GetValueAsDouble()*60;  // Multiply by 60 for Rev/Sec to Rev/Min
}

/** Callback for setting duty cycle of motor */
void dlib::KrakenX60Motor::FetchDutyCycleFromSLCallback()
{
    talonController.SetControl(dutyCycleControl.WithOutput(*finalCreateInfo.dutyCycleCallback));
}

/** Set duty cycle of motor */
void dlib::KrakenX60Motor::SetDutyCycle(double DC)
{
    talonController.SetControl(dutyCycleControl.WithOutput(DC));
}

/** Set brake mode of motor */
void dlib::KrakenX60Motor::SetBrakeMode(bool isBrakeMode)
{
    dutyCycleControl.WithOverrideBrakeDurNeutral(isBrakeMode);
}

/** Stop motor */
void dlib::KrakenX60Motor::StopMotor()
{
    talonController.StopMotor();
}
#include "../include/KrakenTalon.h"

/** constructor for the Kraken Talon */
dlib::KrakenTalon::KrakenTalon(KrakenTalonCreateInfo createInfo)
: talonController{createInfo.canID, createInfo.canbus},
dutyCycleControl{0}
{
    initalizeTalon(createInfo);
}

/** Not useful now, but was created because there used to be multiple constructors */
void dlib::KrakenTalon::initalizeTalon(KrakenTalonCreateInfo createInfo)
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
void dlib::KrakenTalon::getPositionCallback()
{  
    *finalCreateInfo.getPositionCallback = talonController.GetPosition().GetValueAsDouble();
}

/** Callback for getting velocity of motor */
void dlib::KrakenTalon::getVelocityCallback()
{  
    *finalCreateInfo.getVelocityCallback = talonController.GetVelocity().GetValueAsDouble()*60;  // Multiply by 60 for Rev/Sec to Rev/Min
}

/** Callback for setting duty cycle of motor */
void dlib::KrakenTalon::setDutyCycleCallback()
{
    talonController.SetControl(dutyCycleControl.WithOutput(*finalCreateInfo.setDutyCycleCallback));
}

/** set duty cycle of motor */
void dlib::KrakenTalon::setDutyCycle(double DC)
{
    talonController.SetControl(dutyCycleControl.WithOutput(DC));
}

/** set brake mode of motor */
void dlib::KrakenTalon::setBrakeMode(bool isBrakeMode)
{
    dutyCycleControl.WithOverrideBrakeDurNeutral(isBrakeMode);
}

/** stop motor */
void dlib::KrakenTalon::stopMotor()
{
    talonController.StopMotor();
}
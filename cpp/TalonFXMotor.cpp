#include "../include/TalonFXMotor.h"

/** constructor for the Kraken Talon */
dlib::TalonFXMotor::TalonFXMotor(TalonFXMotorCreateInfo createInfo)
: talonController{createInfo.canID, createInfo.canbusName},
  dutyCycleControl{0},
  disconectedCANAlert{alertGroupName, createInfo.displayName, frc::Alert::AlertType::kWarning}
{
    // Initalize Connectivity Error to False
    disconectedCANAlert.Set(false);
    
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
void dlib::TalonFXMotor::SendPositionToSLCallback()
{  
    *finalCreateInfo.positionCallback = talonController.GetPosition().GetValueAsDouble();
}

/** Callback for getting velocity of motor */
void dlib::TalonFXMotor::SendVelocityToSLCallback()
{  
    *finalCreateInfo.velocityCallback = talonController.GetVelocity().GetValueAsDouble()*60;  // Multiply by 60 for Rev/Sec to Rev/Min
}

/** Callback for setting duty cycle of motor */
void dlib::TalonFXMotor::FetchDutyCycleFromSLCallback()
{
    talonController.SetControl(dutyCycleControl.WithOutput(*finalCreateInfo.dutyCycleCallback));
}

/** Set duty cycle of motor */
void dlib::TalonFXMotor::SetDutyCycle(double DC)
{
    talonController.SetControl(dutyCycleControl.WithOutput(DC));
}

/** Set brake mode of motor */
void dlib::TalonFXMotor::SetBrakeMode(bool isBrakeMode)
{
    dutyCycleControl.WithOverrideBrakeDurNeutral(isBrakeMode);
}

/** Stop motor */
void dlib::TalonFXMotor::StopMotor()
{
    talonController.StopMotor();
}

/** Is motor connected to CAN */
void dlib::TalonFXMotor::UpdateCANConnectionAlert()
{
    disconectedCANAlert.Set(!talonController.IsConnected());
}


// CREATE INFO FUNCTIONS

dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::getDefaultCreateInfo()
{
    TalonFXMotorCreateInfo modifiedCreateInfo;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__canID New CAN ID value @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetCanID(int __IN__canID)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.canID = __IN__canID;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__canbusName New smart current limit value @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetCanbusName(const char* __IN__canbusName)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.canbusName = __IN__canbusName;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__isReversed New motor reversal value @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetIsReversed(bool __IN__isReversed)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.isReversed = __IN__isReversed;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__supplyCurrentLimit New supply current limit value @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetSupplyCurrentLimit(double __IN__supplyCurrentLimit)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.supplyCurrentLimit = __IN__supplyCurrentLimit;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__openLoopRampPeriod New open loop ramp period value @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetOpenLoopRampPeriod(double __IN__openLoopRampPeriod)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.openLoopRampPeriod = __IN__openLoopRampPeriod;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__enableFOC If true, field oriented control is enabled @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetEnableFOC(bool __IN__enableFOC)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.enableFOC = __IN__enableFOC;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__motorType Type of motor this controller is plugged into @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetMotorType(TalonFXMotorType __IN__motorType)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.motorType = __IN__motorType;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetDutyCycleCallback(double* __IN__dutyCycleCallback)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.dutyCycleCallback = __IN__dutyCycleCallback;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__velocityCallback New angular velocity callback pointer @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetVelocityCallback(double* __IN__velocityCallback)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.velocityCallback = __IN__velocityCallback;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__positionCallback New angular position callback pointer @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetPositionCallback(double* __IN__positionCallback)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.positionCallback = __IN__positionCallback;
    return modifiedCreateInfo;
}

/** Modify Create Info @param __IN__displayName New display name for Alerts @return Modified copy of create info*/
dlib::TalonFXMotorCreateInfo dlib::TalonFXMotorCreateInfo::SetDisplayName(std::string __IN__displayName)
{
    TalonFXMotorCreateInfo modifiedCreateInfo = *this;
    modifiedCreateInfo.displayName = __IN__displayName;
    return modifiedCreateInfo;
}
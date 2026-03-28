#pragma once
// Just to intelisense sees the header file
#if false
#include "../../include/TalonFX/TalonFXMotor.h"
#endif

namespace dlib::TalonFX
{

/** constructor for the Kraken Talon */
template <ValidControlType CTO>
TalonFXMotor<CTO>::TalonFXMotor(MotorCreateInfo createInfo)
: CTO(std::get<typename CTO::CreateInfoType>(createInfo))
, talonController{std::get<typename CTO::CreateInfoType>(createInfo).canID, std::get<typename CTO::CreateInfoType>(createInfo).canbus}
, talonConfigurator{&talonController.GetConfigurator()}
, disconnectedCANAlert{alertGroupName, CTO::ControlCreateInfo.displayName, frc::Alert::AlertType::kWarning}
{
    
    // Initalize Connectivity Error to False
    disconnectedCANAlert.Set(false);

    // Set Direction
    ctre::phoenix6::configs::MotorOutputConfigs motorDirectionConfig;
    motorDirectionConfig.Inverted = CTO::ControlCreateInfo.isReversed ? 
        ctre::phoenix6::signals::InvertedValue::CounterClockwise_Positive : 
        ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
    talonConfigurator->Apply(motorDirectionConfig);

    // Set Open Loop Ramp Period
    ctre::phoenix6::configs::OpenLoopRampsConfigs rampPeriodConfig;
    units::time::second_t openLoopRampPeriod_time {CTO::ControlCreateInfo.openLoopRampPeriod};
    rampPeriodConfig.DutyCycleOpenLoopRampPeriod = openLoopRampPeriod_time;
    talonConfigurator->Apply(rampPeriodConfig);

    // Set Current Limiting Configuration
    ctre::phoenix6::configs::CurrentLimitsConfigs currentLimitsConfig;
    units::current::ampere_t supplyCurrentLimit_amps {CTO::ControlCreateInfo.supplyCurrentLimit};
    currentLimitsConfig.SupplyCurrentLimit = supplyCurrentLimit_amps;
    currentLimitsConfig.SupplyCurrentLimitEnable = true;
    talonConfigurator->Apply(currentLimitsConfig);

    CTO::DeviceControlConfigure(talonConfigurator);

    // Store for later use
    finalCreateInfo = createInfo;
}

/** Callback for getting position of motor */
template <ValidControlType CTO>
void TalonFXMotor<CTO>::SendPositionToSLCallback()
{  
    *CTO::ControlCreateInfo.positionCallback = talonController.GetPosition().GetValueAsDouble();
}

/** Callback for getting velocity of motor */
template <ValidControlType CTO>
void TalonFXMotor<CTO>::SendVelocityToSLCallback()
{  
    *CTO::ControlCreateInfo.velocityCallback = talonController.GetVelocity().GetValueAsDouble()*60;  // Multiply by 60 for Rev/Sec to Rev/Min
}

template <ValidControlType CTO>
void TalonFXMotor<CTO>::ControlLoop()
{
    bool temp = false;
    talonController.SetControl(CTO::GetControl(temp));
}

/** Stop motor */
template <ValidControlType CTO>
void TalonFXMotor<CTO>::StopMotor() { talonController.StopMotor(); }

/** Is motor connected to CAN */
template <ValidControlType CTO>
void TalonFXMotor<CTO>::UpdateCANConnectionAlert() { disconnectedCANAlert.Set(!talonController.IsConnected()); }

template <ValidControlType CTO>
bool TalonFXMotor<CTO>::IsAngularPositionRequested() { return CTO::ControlCreateInfo.positionCallback != nullptr; }

template <ValidControlType CTO>
bool TalonFXMotor<CTO>::IsAngularVelocityRequested() { return CTO::ControlCreateInfo.velocityCallback != nullptr; }

}; // dlib::TalonFX
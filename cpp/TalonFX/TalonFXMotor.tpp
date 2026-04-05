#pragma once
// Just so intelisense can see the header and highlight correctly
#include "../../include/TalonFX/TalonFXMotor.h"

namespace dlib::TalonFX
{

/** constructor for the Kraken Talon */
template <class ControlObjClass, class CreateInfoClass>
template<typename... ObjArgs>
BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::BaseTalonFXMotor(CreateInfoClass createInfo, ObjArgs&&... args)
: savedCreateInfo{std::move(createInfo)}
, ControlObj{std::forward<ObjArgs>(args)...}
, talonController{savedCreateInfo.canID, savedCreateInfo.canbus}
, talonConfigurator{talonController.GetConfigurator()}
, disconnectedCANAlert{alertGroupName, savedCreateInfo.displayName, frc::Alert::AlertType::kWarning}
{
    ctre::phoenix6::configs::TalonFXConfiguration CompleteConfig;
    // Initalize Connectivity Error to False
    disconnectedCANAlert.Set(false);

    // Set Direction
    ctre::phoenix6::configs::MotorOutputConfigs motorDirectionConfig;
    motorDirectionConfig.Inverted = savedCreateInfo.isReversed ? 
        ctre::phoenix6::signals::InvertedValue::CounterClockwise_Positive : 
        ctre::phoenix6::signals::InvertedValue::Clockwise_Positive;
    CompleteConfig.WithMotorOutput(motorDirectionConfig);

    // Set Open Loop Ramp Period
    ctre::phoenix6::configs::OpenLoopRampsConfigs rampPeriodConfig;
    units::time::second_t openLoopRampPeriod_time {savedCreateInfo.openLoopRampPeriod};
    rampPeriodConfig.DutyCycleOpenLoopRampPeriod = openLoopRampPeriod_time;
    CompleteConfig.WithOpenLoopRamps(rampPeriodConfig);

    // Set Current Limiting Configuration
    ctre::phoenix6::configs::CurrentLimitsConfigs currentLimitsConfig;
    units::current::ampere_t supplyCurrentLimit_amps {savedCreateInfo.supplyCurrentLimit};
    currentLimitsConfig.SupplyCurrentLimit = supplyCurrentLimit_amps;
    currentLimitsConfig.SupplyCurrentLimitEnable = true; 
    CompleteConfig.WithCurrentLimits(currentLimitsConfig);
    
    talonConfigurator.Apply(CompleteConfig);
}

/** Callback for getting position of motor */
template <class ControlObjClass, class CreateInfoClass>
void BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::SendPositionToSLCallback()
{  
    if(!savedCreateInfo.positionCallback) return;
    *savedCreateInfo.positionCallback = talonController.GetPosition().GetValueAsDouble();
}

/** Callback for getting velocity of motor */
template <class ControlObjClass, class CreateInfoClass>
void BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::SendVelocityToSLCallback()
{  
    if(!savedCreateInfo.velocityCallback) return;
    *savedCreateInfo.velocityCallback = talonController.GetVelocity().GetValueAsDouble()*60;  // Multiply by 60 for Rev/Sec to Rev/Min
}

template <class ControlObjClass, class CreateInfoClass>
void BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::ControlLoop()
{
    UpdateControl();
    talonController.SetControl(ControlObj);
}

/** Stop motor */
template <class ControlObjClass, class CreateInfoClass>
void BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::StopMotor() { talonController.StopMotor(); }

/** Is motor connected to CAN */
template <class ControlObjClass, class CreateInfoClass>
void BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::UpdateCANConnectionAlert() { disconnectedCANAlert.Set(!talonController.IsConnected()); }

template <class ControlObjClass, class CreateInfoClass>
bool BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::IsAngularPositionRequested() { return savedCreateInfo.positionCallback != nullptr; }

template <class ControlObjClass, class CreateInfoClass>
bool BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::IsAngularVelocityRequested() { return savedCreateInfo.velocityCallback != nullptr; }

template <class ControlObjClass, class CreateInfoClass>
bool BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::HasControlLoop() {return activeControlLoop;}

template <class ControlObjClass, class CreateInfoClass>
void BaseTalonFXMotor<ControlObjClass, CreateInfoClass>::SetBrakeMode(bool isBrakeMode)
{
    if constexpr (requires { std::declval<ControlObjClass>().WithOverrideBrakeDurNeutral(isBrakeMode); })
    {
        ControlObj.WithOverrideBrakeDurNeutral(isBrakeMode);
    }
}


}; // dlib::TalonFX
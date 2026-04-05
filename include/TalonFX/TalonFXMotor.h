#pragma once
// Disables the header for reading in the tpp file
#define TALON_FX_MOTOR_H

// Std
#include <optional>
#include <cstdint>
#include <string>
#include <variant>
#include <cstdint>

// Frc
#include <frc/Alert.h>

// Ctre
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/core/CoreTalonFX.hpp>
#include <ctre/phoenix6/StatusSignal.hpp>
#include <ctre/phoenix6/configs/MotorOutputConfigs.hpp>
#include <ctre/phoenix6/configs/CurrentLimitsConfigs.hpp>
#include <ctre/phoenix6/CANBus.hpp>

// Local
#include "../../include/TalonFX/TalonFXCreateInfo.h"

namespace dlib::TalonFX
{

/** A single TalonFX motor */
template <class ControlObjClass, class CreateInfoClass>
class BaseTalonFXMotor
{
public:
    /** Constructor for the TalonFX motor */
    template<typename... ObjArgs>
    BaseTalonFXMotor(CreateInfoClass createInfo, ObjArgs&&... args);
    
    /** Disable copy & move constructors */
    BaseTalonFXMotor(const BaseTalonFXMotor &obj) = delete;
    BaseTalonFXMotor(BaseTalonFXMotor&& other) = delete;
    virtual ~BaseTalonFXMotor() = default;
    /** Callback for getting position of motor */
    void SendPositionToSLCallback();
    
    /** Callback for getting velocity of motor */
    void SendVelocityToSLCallback();
    
    void ControlLoop();

    /** Stop motor */
    void StopMotor(); 

    /** Is motor connected to CAN */
    void UpdateCANConnectionAlert();

    bool IsAngularPositionRequested();
    bool IsAngularVelocityRequested();
    bool HasControlLoop();

    void SetBrakeMode(bool isBrakeMode);
    
    virtual void UpdateControl() = 0;
    

protected:
    CreateInfoClass savedCreateInfo;
    ControlObjClass ControlObj;
private:
    ctre::phoenix6::hardware::TalonFX talonController;
protected:
    ctre::phoenix6::configs::TalonFXConfigurator& talonConfigurator;
    bool activeControlLoop = true;
private:
/** CAN connection warning */
frc::Alert disconnectedCANAlert;

/** Group name for all TalonFX CAN Alerts */
static constexpr std::string_view alertGroupName = "CAN Connection Alerts";

};

};

#include "../../cpp/TalonFX/TalonFXMotor.tpp"
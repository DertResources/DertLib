#pragma once
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
#include "../../include/TalonFX/TalonFXControlTypes.h"

namespace dlib::TalonFX
{

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };

/** A single TalonFX motor */

template <class T>
concept ValidControlType = requires(T t,
                                    bool boolType,
                                    ctre::phoenix6::hardware::TalonFX talon,
                                    ctre::phoenix6::configs::TalonFXConfigurator* talonConfigurator)
{
    talon.SetControl(t.GetControl(boolType));
    {t.SetBrakeMode(boolType)} ->std::same_as<void>;
    {t.DeviceControlConfigure(talonConfigurator)} ->std::same_as<void>;
    // requires std::derived_from<std::remove_cvref_t<decltype(t.ControlCreateInfo)>, BaseMotorCreateInfo>;
};

template <ValidControlType CTO>
class TalonFXMotor : public CTO
{
public:
    /** Constructor for the TalonFX motor */
    TalonFXMotor(MotorCreateInfo createInfo);
    
    /** Disable move constructor */
    TalonFXMotor(TalonFXMotor&& other) = delete;

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
    
private:
    MotorCreateInfo finalCreateInfo;

    ctre::phoenix6::hardware::TalonFX talonController;
    
    ctre::phoenix6::configs::TalonFXConfigurator* talonConfigurator;
    
    /** CAN connection warning */
    frc::Alert disconnectedCANAlert;
    
    /** Group name for all TalonFX CAN Alerts */
    static constexpr std::string_view alertGroupName = "CAN Connection Alerts";

};

};

#include "../../cpp/TalonFX/TalonFXMotor.tpp"
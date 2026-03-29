#pragma once

// Std
#include <functional>

// ctre
#include <ctre/phoenix6/controls/DutyCycleOut.hpp>
#include <ctre/phoenix6/controls/VelocityDutyCycle.hpp>
#include <ctre/phoenix6/controls/StrictFollower.hpp>
#include <ctre/phoenix6/core/CoreTalonFX.hpp>

// Units
#include <units/time.h>
#include <units/current.h>
#include <units/frequency.h>
#include <units/angular_acceleration.h>
#include <units/angular_velocity.h>

// Local
#include "../../include/TalonFX/TalonFXCreateInfo.h"

// Unit Small
using AngularAccel = units::angular_acceleration::turns_per_second_squared_t;
using AngularVel = units::angular_velocity::turns_per_second_t;
using Hz = units::frequency::Hz;

// Control Obj Small
using DCControlObj = ctre::phoenix6::controls::DutyCycleOut;
using VelControlObj = ctre::phoenix6::controls::VelocityDutyCycle;
using FollowControlObj = ctre::phoenix6::controls::StrictFollower;
using Configurator = ctre::phoenix6::configs::TalonFXConfigurator;

namespace dlib::TalonFX 
{

// // // // // // // // // // // // // // // // // // // // // //
class DutyCycleControl
{
public:
    DutyCycleControl(TalonFX::DutyCycleCreateInfo createInfo);
    void DeviceControlConfigure(Configurator* talonConfigurator);
    void SetBrakeMode(bool &isBrakeMode);
    DCControlObj GetControl(bool& selfDestruct);
    DutyCycleCreateInfo ControlCreateInfo;
    using CreateInfoType = DutyCycleCreateInfo;

private:
    DCControlObj ControlObj;
};
// // // // // // // // // // // // // // // // // // // // // //
class VelocityControl
{
public:
    VelocityControl(TalonFX::VelocityCreateInfo createInfo);
    void DeviceControlConfigure(Configurator* talonConfigurator);
    void SetBrakeMode(bool &isBrakeMode);
    VelControlObj GetControl(bool& selfDestruct);
    VelocityCreateInfo ControlCreateInfo;
    using CreateInfoType = VelocityCreateInfo;
private:
    VelControlObj ControlObj;
    ctre::phoenix6::configs::Slot0Configs slot0;
    Configurator* talonConfigurator;
    bool isFFEnabled = false;

};
// // // // // // // // // // // // // // // // // // // // // //
class FollowerControl
{
public:
    FollowerControl(TalonFX::FollowerCreateInfo createInfo);
    void DeviceControlConfigure(Configurator* talonConfigurator);
    void SetBrakeMode(bool &isBrakeMode);
    FollowControlObj GetControl(bool& selfDestruct);
    FollowerCreateInfo ControlCreateInfo;
    using CreateInfoType = FollowerCreateInfo;
private:
    FollowControlObj ControlObj;
};
// // // // // // // // // // // // // // // // // // // // // //

typedef std::variant<DutyCycleControl, VelocityControl, FollowerControl> ControlTypes;

}; //namespace dlib::TalonFX

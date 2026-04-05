#pragma once

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
#include "../../include/TalonFX/TalonFXMotor.h"
#include "../../include/TalonFX/TalonFXPidHandler.h"

// Unit Small
using AngularAccel = units::angular_acceleration::turns_per_second_squared_t;
using AngularVel = units::angular_velocity::turns_per_second_t;
using Hz = units::frequency::Hz;

namespace dlib::TalonFX 
{

// // // // // // // // // // // // // // // // // // // // // //
class DutyCycleControl : public BaseTalonFXMotor<ctre::phoenix6::controls::DutyCycleOut, DutyCycleCreateInfo>
{
public:
    DutyCycleControl(DutyCycleCreateInfo createInfo);
    void UpdateControl() override;
};
// // // // // // // // // // // // // // // // // // // // // //
class VelocityControl : public BaseTalonFXMotor<ctre::phoenix6::controls::VelocityDutyCycle, VelocityCreateInfo>
                      , public PidHandler      <ctre::phoenix6::controls::VelocityDutyCycle, VelocityCreateInfo>
{
public:
    VelocityControl(VelocityCreateInfo createInfo);
    void UpdateControl() override;
private:
    bool isFFEnabled = false;
};
// // // // // // // // // // // // // // // // // // // // // //
class FollowerControl : public BaseTalonFXMotor<ctre::phoenix6::controls::StrictFollower, FollowerCreateInfo>
{
public:
    FollowerControl(FollowerCreateInfo createInfo);
    void UpdateControl() override;
};
// // // // // // // // // // // // // // // // // // // // // //

using TalonFXMotor = std::variant<DutyCycleControl, VelocityControl, FollowerControl> ;

template<class T> struct ControlTypeFrom;
template<> struct ControlTypeFrom<DutyCycleCreateInfo> { using type = DutyCycleControl; };
template<> struct ControlTypeFrom<VelocityCreateInfo>  { using type = VelocityControl;  };
template<> struct ControlTypeFrom<FollowerCreateInfo>  { using type = FollowerControl;  };

}; //namespace dlib::TalonFX

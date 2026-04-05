#pragma once

// Std
#include <algorithm>
#include <concepts>
#include <vector>
#include <functional>
#include <variant>
#include <concepts>
#include <type_traits>
// Local
#include "../../include/TalonFX/TalonFXControlTypes.h"

// Ctre
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/core/CoreTalonFX.hpp>

namespace dlib::TalonFX {

template <class T>
concept ControlTypeClass = requires(T t, bool brakeMode)
{
    { t.UpdateCANConnectionAlert()  } -> std::same_as<void>;
    { t.StopMotor()                 } -> std::same_as<void>;
    { t.SetBrakeMode(brakeMode)     } -> std::same_as<void>;
    { t.SendPositionToSLCallback()  } -> std::same_as<void>;
    { t.SendVelocityToSLCallback()  } -> std::same_as<void>;
    { t.HasControlLoop()            } -> std::same_as<bool>;
    { t.IsAngularPositionRequested()} -> std::same_as<bool>;
    { t.IsAngularVelocityRequested()} -> std::same_as<bool>;
    { t.ControlLoop()               } -> std::same_as<void>;
};


/**
 * TalonFXMotorGroup is a list of TalonFXMotors
 * this is a wrapper to automate a large amount of motor control
 */
class TalonFXMotorGroup
{
public:
    /** Constructor for the TalonFX motor group 
     * @param createInfos Array of TalonFX motor create infos
     */
     
    TalonFXMotorGroup(std::initializer_list<std::variant<
        MotorCreateInfo<DutyCycleCreateInfo>,
        MotorCreateInfo<VelocityCreateInfo>,
        MotorCreateInfo<FollowerCreateInfo>
        >> createInfos);
    
    /** Stop all motors */
    void Stop();
    
    /** Set the brake mode when idle (coast / break) for all motors 
     * @param isBrakeMode The mode the motors will be in when command is zero.
     * true for break, false for coast
     */
    void SetBrakeModeWhenIdle(bool isBrakeMode);        

    // // // // // // // // // // // // // //

    /** Pull positions and velocities from motor controllers and push them into simulink */
    void SendSensorDataToSL();

    /** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
    void FetchMotorCommandsFromSL();

    // // // // // // // // // // // // // //

    /** Pull motor angular velocities from motor controllers and push them into simulink */
    void SendVelocityValuesToSL();

    /** Pull motor angular positions from motor controllers and push them into simulink */
    void SendPositionValuesToSL();

    /** Update the warnings of CAN connections */
    void UpdateMotorCANConnectionAlerts();

    // TO BE FIXED LATER WITH ROBOT DECICE OVERHAUL, FINE RN AS ROBOT NEVER DESTRUCTS
    /** List of all instaniated motor groups */
    static std::vector<TalonFXMotorGroup*> allTalonFXMotorGroups; 
private:
    /** Add all callbacks to the callback vectors
     * @param motor Reference to TalonFXMotor object
     */
    template <ControlTypeClass T>
    void AddCallbacks(T& motor);

    // // // // // // // // // // // // // //

    /** Vector of all motors in this set */

    std::vector<std::unique_ptr<TalonFXMotor>> motorSet;

    /** Holds functions that update the angular position values for the motors */
    std::vector<std::function<void()>> velocityCallbacks;
    
    /** Holds functions that update the angular position values for the motors */
    std::vector<std::function<void()>> positionCallbacks;
    
    /** Holds functions to update the duty cycle of the motors */
    std::vector<std::function<void()>> ControlCallbacks;
};

};
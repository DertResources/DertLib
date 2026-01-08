#pragma once

// Std
#include <algorithm>
#include <vector>
#include <functional>

// Local
#include "../include/TalonFXMotor.h"

// Ctre
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/core/CoreTalonFX.hpp>

namespace dlib {
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
        TalonFXMotorGroup(std::initializer_list<TalonFXMotorCreateInfo> createInfos);
        
        /** Stop all motors */
        void Stop();
        
        /** Set the duty cycle of all motors
         * @param dutyCycle Duty cycle for all motors
         */
        void Set(double dutyCycle);

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

    private:
        /** Add all callbacks to the callback vectors
         * @param motor Reference to TalonFXMotor object
         */
        void AddCallbacks(TalonFXMotor& motor);

        // // // // // // // // // // // // // //

        /** Vector of all motors in this set */
        std::vector<std::unique_ptr<TalonFXMotor>> motorSet;

        /** Holds functions that update the angular position values for the motors */
        std::vector<std::function<void()>> velocityCallbacks;
        
        /** Holds functions that update the angular position values for the motors */
        std::vector<std::function<void()>> positionCallbacks;
        
        /** Holds functions to update the duty cycle of the motors */
        std::vector<std::function<void()>> dutyCycleCallbacks;
    };
};
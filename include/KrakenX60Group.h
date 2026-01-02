#pragma once

// Std
#include <algorithm>
#include <vector>
#include <functional>

// Local
#include "../include/KrakenX60Motor.h"

// Ctre
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/core/CoreTalonFX.hpp>

namespace dlib {
    /**
     * KrakenX60Group is a list of KrakenX60Motors
     * this is a wrapper to automate a large amount of motor control
     */
    class KrakenX60Group
    {
    public:
        /** Constructor for the kraken set 
         * @param createInfos array of kraken create infos
         */
        KrakenX60Group(std::initializer_list<KrakenX60MotorCreateInfo> createInfos);
        
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

    private:
        /** Add all callbacks to the callback vectors
         * @param motor Reference to KrakenX60Motor object
         */
        void AddCallbacks(KrakenX60Motor& motor);

        // // // // // // // // // // // // // //

        /** Vector of all motors in this set */
        std::vector<std::unique_ptr<KrakenX60Motor>> motorSet;

        /** Holds functions that update the angular position values for the motors */
        std::vector<std::function<void()>> velocityCallbacks;
        
        /** Holds functions that update the angular position values for the motors */
        std::vector<std::function<void()>> positionCallbacks;
        
        /** Holds functions to update the duty cycle of the motors */
        std::vector<std::function<void()>> dutyCycleCallbacks;
    };
};
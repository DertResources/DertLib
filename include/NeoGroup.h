#pragma once

// Std
#include <algorithm>
#include <vector>
#include <functional>
#include <memory>

// Local
#include "../include/NeoMotor.h"

// Rev
#include <rev/SparkMax.h>
#include <rev/config/SparkMaxConfig.h>
#include <rev/SparkRelativeEncoder.h>

namespace dlib {
    class NeoGroup
    {
    public:
        /** Constructor for the Neo set 
         *  @param createInfos Array of Neo create infos
        */
        NeoGroup(std::initializer_list<NeoMotorCreateInfo> createInfos);
        
        // // // // // // // // // // // // // //

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
         * @param motor reference to NeoMotor object
         */
        void AddCallbacks(NeoMotor& motor);

        /** Vector of all motors in this set */
        std::vector<std::unique_ptr<NeoMotor>> motorSet;

        // // // // // // // // // // // // // //
        
        /** Holds functions that update the angular position values for the motors */
        std::vector<std::function<void()>> velocityCallbacks;
        
        /** Holds functions that update the angular position values for the motors */
        std::vector<std::function<void()>> positionCallbacks;
        
        /** Holds functions to update the duty cycle of the motors */
        std::vector<std::function<void()>> dutyCycleCallbacks;
    };
};
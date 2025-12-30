#pragma once

//std
#include <algorithm>
#include <vector>
#include <functional>
//local
#include "../include/KrakenTalon.h"
//stre
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/core/CoreTalonFX.hpp>

namespace dlib {
    /**
     * Kraken Set is a list of KrakenTalons
     * this is a wrapper to automate a large amount of motor control
     */
    class KrakenSet
    {
    public:
        /** Constructor for the kraken set 
         * @param createInfos array of kraken create infos
        */
        KrakenSet(std::initializer_list<KrakenTalonCreateInfo> createInfos);
        
        /** Stop all motors */
        void stop();
        
        /** Set the duty cycle of all motors
         * @param dutyCycle duty cycle for all motors
         */
        void set(double dutyCycle);

        /** Set the brake mode when idle (coast / break) for all motors 
         * @param isBrakeMode the mode the motors will be in when command is zero.
         * true for break, false for coast
         */
        void setBrakeModeWhenIdle(bool isBrakeMode);        

        /** Pull positions and velocities from motor controllers and push them into simulink */
        void pushData();

        /** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
        void pullCommands();

        /** Pull motor angular velocities from motor controllers and push them into simulink */
        void pushVelocities();

        /** Pull motor angular positions from motor controllers and push them into simulink */
        void pushPositions();
    private:
        /** Add all callbacks to the callback vectors
         * @param motor reference to KrakenTalon object
         */
        void addCallbacks(KrakenTalon& motor);

        /** Vector of all motors in this set */
        std::vector<std::unique_ptr<KrakenTalon>> motorSet;

        /** Holds functions that update the angular position values for the motors */
        std::vector<std::function<void()>> VelocityCallbacks;
        
        /** Holds functions that update the angular position values for the motors */
        std::vector<std::function<void()>> PositionCallbacks;
        
        /** Holds functions to update the duty cycle of the motors */
        std::vector<std::function<void()>> DutyCycleCallbacks;
    };
};
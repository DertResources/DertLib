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
    class KrakenSet
    {
    public:
        KrakenSet(std::initializer_list<KrakenTalonCreateInfo> createInfos);
        void stop();
        void set(double dutyCycle);
        void setBrakeModeWhenIdle(bool isBrakeMode);
        void pullVelocities();
        void pullPositions();
        void pushData();
        void pullCommands();
        void pushVelocities();
        void pushPositions();
    private:
        void addCallbacks(KrakenTalon& motor);
        std::vector<std::unique_ptr<KrakenTalon>> motorSet;
        std::vector<std::function<void()>> VelocityCallbacks;
        std::vector<std::function<void()>> PositionCallbacks;
        std::vector<std::function<void()>> DutyCycleCallbacks;
    };
};
#pragma once

//Local
#include "../include/Component.h"

//Std
#include <vector>
namespace dlib {
    class HighFrequencyComponent : public dlib::Component
    {
    public:
        static std::vector<HighFrequencyComponent*> AllCreatedHighFrequencyComponents;

        virtual void HighFrequencyPreStep() = 0;
        virtual void HighFrequencyPostStep() = 0;
    };
};
#pragma once

// Std
#include <vector>

// Local
#include "../include/Component.h"

namespace dlib {

    /**
     * Inherits the Component class, this class has two new pure virtual
     * callback functions, primerialy for the 250Hz odometry loop
     */
    class HighFrequencyComponent : public dlib::Component
    {
    public:
        /** A super constructor that holds together all child "Component Classes" */
        HighFrequencyComponent();

        /**
         * A vector of pointers to each component instansiated that is a child to
         * this class. This vector is itterated through in robot.h
         */
        static std::vector<HighFrequencyComponent*> allCreatedHighFrequencyComponents;

        /**
         * Component specific callback that runs before the high frequency simulink
         * step function and in between steps at a higher rate than the step function
         */
        virtual void HighFrequencyPreStepCallback() = 0;

        /**
         * Component specific callback that runs after the high frequency simulink
         * step function and in between steps at a higher rate than the step function
         */
        virtual void HighFrequencyPostStepCallback() = 0;
    };
};
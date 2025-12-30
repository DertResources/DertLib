#pragma once
#include <functional>
#include <vector>
#include <iostream>

namespace dlib {

    /**
     * Holds PreStep and PostStep callback for component classes.
     */
    class Component
    {
    public:
        Component();

        /** 
         * Contains references to all created components.
         * This vector is called from robot.h to itterate
         * through all callbacks. 
         */
        static std::vector<Component*> AllCreatedComponents; 
        
        /**
         * Component specific callback that runs before the simulink step function
         */
        virtual void PreStepCallback() = 0;

        /**
         * Component specific callback that runs after the simulink step function
         */
        virtual void PostStepCallback() = 0;
    };
};
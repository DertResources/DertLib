#pragma once

// Std
#include <vector>
#include <utility>
#include <string>
#include <functional>

// Frc
#include <frc/smartdashboard/SendableChooser.h>
#include <frc/smartdashboard/SmartDashboard.h>

namespace dlib {
    /**
     * Automation for the chooser class (dropdown visual on elastic GUI).
     * Chooser takes in a map of string names and integer values.
     * Values output double because that is the default type for simulink in/out
     */
    class Chooser
    {
    public:
        /**
         * Manual chooser constructor, the user must grab results each loop
         * @param chooserName Title of the chooser
         * @param chooserMap Map of options in the chooser to the ints that are sent to simulink
         */
        Chooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap);

        /**
         * Simulink based constructor, this will automatically send user selections to simulink
         * @param chooserName Title of the chooser
         * @param chooserMap Map of options in the chooser to the ints that are sent to simulink
         * @param simulinkPointer Pointer to the simulink U port.
         */
        Chooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap, double* simulinkPointer);

        /**
         * Gets the current user selected option and returns the int that maps to that
         */
        double GetSelectedKey();

        /**
         * Gets the int that matches the input key
         * @param currentSelection name of option in chooser, or the 'key'
         * @return the mapped integer for the string key 'currentSelection'
         * @warning the default value if the current selected is not found is -1
         */
        double GetSelectedKey(std::string currentSelection);

    private:
        /**
         * Setup chooser graphic in elastic
         * @param chooserName Title of chooser in Elastic
         * @param chooserMap  Map of options in the chooser, and the output int for each
         */
        void InitChooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap);

        /** WPILib Chooser Object */
        frc::SendableChooser<std::string> m_chooser;
        
        /** The map of string keys and integer value pairs */
        std::vector<std::pair<std::string, int>> chooserMap;

        /** Pointer to the input simulink port */
        double* chooserSelectedKeyPointer;
    };
};

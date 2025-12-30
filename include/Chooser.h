#pragma once

//std
#include <vector>
#include <utility>
#include <string>
#include <functional>

// frc
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
        Chooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap);
        Chooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap, double* simulinkPointer);

        double GetSelectedKey();

        double GetSelectedKey(std::string currentSelection);

    private:
        void InitChooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap);

        frc::SendableChooser<std::string> m_chooser;
        std::vector<std::pair<std::string, int>> chooserMap;

        double* chooserSelectedKeyPointer;
    };
};

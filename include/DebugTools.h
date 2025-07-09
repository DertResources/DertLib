#pragma once

// Local
#include <frc/smartdashboard/SmartDashboard.h>

namespace dlib {
    class DebugTools
    {
    public:
        template<typename T>
        typename std::enable_if<std::is_arithmetic<T>::value>::type
        static DebugPrintNumber(std::string_view keyName, T value)
        {
            frc::SmartDashboard::PutString(keyName, std::to_string(value));
        }

        template<typename T>
        typename std::enable_if<std::is_arithmetic<T>::value>::type
        static DebugPrintNumberVector(std::string_view keyName, std::vector<T> vector)
        {
            std::vector<std::string> newVec;
            for(T element : vector)
                newVec.push_back(std::to_string(element));

            frc::SmartDashboard::PutStringArray(keyName, std::span<const std::string>(newVec.data(), newVec.size()));
        }
    private:
    };
};
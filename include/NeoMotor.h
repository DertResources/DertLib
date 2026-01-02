#pragma once

// Std
#include <optional>
#include <cstdint>

// Rev
#include <rev/SparkMax.h>
#include <rev/config/SparkMaxConfig.h>
#include <rev/SparkRelativeEncoder.h>

namespace dlib {
    /** Create info for Neo Spark */
    struct NeoMotorCreateInfo
    {
        // Motor Properties
        int canID = -1;
        bool isReversed = false;
        uint8_t smartCurrentLimit = 15;
        double secondaryCurrentLimit = 15;
        double openLoopRampRate = 0.00000001;
        // Simulink Port Pointers
        double* dutyCycleCallback = nullptr;
        double* velocityCallback = nullptr;
        double* positionCallback = nullptr;
        rev::spark::SparkMax::MotorType motorType = rev::spark::SparkMax::MotorType::kBrushless;

        /** Modify Create Info @param __IN__canID New CAN ID value @return Modified copy of create info*/
        NeoMotorCreateInfo SetCanID(int __IN__canID)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.canID = __IN__canID;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__isReversed New motor reversal value @return Modified copy of create info*/
        NeoMotorCreateInfo SetIsReversed(bool __IN__isReversed)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.isReversed = __IN__isReversed;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__smartCurrentLimit New smart current limit value @return Modified copy of create info*/
        NeoMotorCreateInfo SetSmartCurrentLimit(uint8_t __IN__smartCurrentLimit)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.smartCurrentLimit = __IN__smartCurrentLimit;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__secondaryCurrentLimit New secondary current limit value @return Modified copy of create info*/
        NeoMotorCreateInfo SetSecondaryCurrentLimit(double __IN__secondaryCurrentLimit)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.secondaryCurrentLimit = __IN__secondaryCurrentLimit;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__openLoopRampRate New open loop ramp rate value @return Modified copy of create info*/
        NeoMotorCreateInfo SetOpenLoopRampRate(double __IN__openLoopRampRate)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.openLoopRampRate = __IN__openLoopRampRate;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
        NeoMotorCreateInfo SetDutyCycleCallback(double* __IN__dutyCycleCallback)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.dutyCycleCallback = __IN__dutyCycleCallback;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__velocityCallback New angular velocity callback pointer @return Modified copy of create info*/
        NeoMotorCreateInfo SetVelocityCallback(double* __IN__velocityCallback)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.velocityCallback = __IN__velocityCallback;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__positionCallback New angular position callback pointer @return Modified copy of create info*/
        NeoMotorCreateInfo SetPositionCallback(double* __IN__positionCallback)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.positionCallback = __IN__positionCallback;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__motorType New motor type value @return Modified copy of create info*/
        NeoMotorCreateInfo SetMotorType(rev::spark::SparkMax::MotorType __IN__motorType)
        {
            NeoMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.motorType = __IN__motorType;
            return modifiedCreateInfo;
        }
    };

    /** A single Neo 1.1 motor */
    class NeoMotor
    {
    public:
        /** Constructor for the Neo Motor */
        NeoMotor(NeoMotorCreateInfo createInfo);

        /** Disable move constructor */
        NeoMotor(NeoMotor&& other) noexcept;

        // // // // // // // // // // // // // //

        /** Callback for getting position of motor */
        void SendPositionToSLCallback();

        /** Callback for getting velocity of motor */
        void SendVelocityToSLCallback();

        /** Callback for setting duty cycle of motor */
        void FetchDutyCycleFromSLCallback();

        // // // // // // // // // // // // // //

        /** Set duty cycle of motor */
        void SetDutyCycle(double DC);  

        /** Set brake mode of motor */
        void SetBrakeMode(bool isBrakeMode);

        /** Stop motor */
        void StopMotor();

        /** Create info for this motor */
        NeoMotorCreateInfo finalCreateInfo{};

    private:
        /** Motor controller object
         * @warning This object has its copy and move constructors disabled
         */
        rev::spark::SparkMax sparkMax;

        /** Internal Encode Object
         * @warning This object has its copy and move constructors disabled
         * @warning This object is only filled when either velocityCallback or positionCallback
         * are not nullptr 
         */
        std::optional<rev::spark::SparkRelativeEncoder> sparkRelEncoder;        
    };
};
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
        int canID;
        bool isReversed = false;
        uint8_t smartCurrentLimit = 15;
        double secondaryCurrentLimit = 15;
        double openLoopRampRate = 0.00000001;
        // Simulink Port Pointers
        double* dutyCycleCallback = nullptr;
        double* velocityCallback = nullptr;
        double* positionCallback = nullptr;
        rev::spark::SparkMax::MotorType motorType = rev::spark::SparkMax::MotorType::kBrushless;

        /** Modifies info in the create info, mainly used for create infos
         * that are similar and share values
         * @param oldCreateInfo Old create info
         * @param canID Can ID
         * @param dutyCycleCallback Pointer to simulink duty cycle value,
         * the variable should follow this pattern &[MODELNAME]_Y.[VARIABLE_NAME]
         * (set to nullptr if not used)
         * @param velocityCallback Pointer to simulink velocity value - 
         * the variable should follow this pattern &[MODELNAME]_U.[VARIABLE_NAME]
         * (set to nullptr if not used)
         * @param positionCallback Pointer to simulink position value -
         * the variable should follow this pattern &[MODELNAME]_U.[VARIABLE_NAME] 
         * (set to nullptr if not used)
         * @return Modified return create info
         */
        static constexpr NeoMotorCreateInfo modifyInfo(
            NeoMotorCreateInfo oldCreateInfo,
            int canID,
            double* dutyCycleCallback = nullptr,
            double* velocityCallback = nullptr,
            double* positionCallback = nullptr)
        {
            auto newCreateInfo = oldCreateInfo;
            newCreateInfo.canID = canID;
            newCreateInfo.dutyCycleCallback = dutyCycleCallback;
            newCreateInfo.velocityCallback = velocityCallback;
            newCreateInfo.positionCallback = positionCallback;
            return newCreateInfo;
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
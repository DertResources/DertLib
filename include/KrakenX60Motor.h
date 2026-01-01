#pragma once
//std
#include <optional>
#include <cstdint>
#include <string>
//units
#include <units/time.h>
#include <units/current.h>
//ctre
#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/core/CoreTalonFX.hpp>
#include <ctre/phoenix6/configs/Configs.hpp>
#include <ctre/phoenix6/StatusSignal.hpp>

namespace dlib {
    /** Create info for Kraken Talon */
    struct KrakenX60MotorCreateInfo
    {
        // Motor Properties
        int canID;
        const char* canbusName;
        bool isReversed = false;
        double supplyCurrentLimit = 15;
        double openLoopRampPeriod = 0;
        bool enableFOC = true;
        // Simulink Port Pointers
        double* dutyCycleCallback = nullptr;
        double* velocityCallback = nullptr;
        double* positionCallback = nullptr;

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
        static constexpr KrakenX60MotorCreateInfo modifyInfo(
            KrakenX60MotorCreateInfo oldCreateInfo,
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

    /** A single KrakenX60 */
    class KrakenX60Motor
    {
    public:
        /** Constructor for the KrakenX60 */
        KrakenX60Motor(KrakenX60MotorCreateInfo createInfo);
        
        /** Disable move constructor */
        KrakenX60Motor(KrakenX60Motor&& other) noexcept;

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
        KrakenX60MotorCreateInfo finalCreateInfo{};

    private:
        /** Motor controller object
         * @warning This object has its copy and move constructors disabled
         */
        ctre::phoenix6::hardware::TalonFX talonController;

        /** Motor control object */
        ctre::phoenix6::controls::DutyCycleOut dutyCycleControl;
    };
};
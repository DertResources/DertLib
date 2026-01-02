#pragma once

// Std
#include <optional>
#include <cstdint>
#include <string>

// Units
#include <units/time.h>
#include <units/current.h>

// Ctre
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

        /** Modify Create Info @param __IN__canID New CAN ID value @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetCanID(int __IN__canID)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.canID = __IN__canID;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__canbusName New smart current limit value @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetCanbusName(const char* __IN__canbusName)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.canbusName = __IN__canbusName;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__isReversed New motor reversal value @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetIsReversed(bool __IN__isReversed)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.isReversed = __IN__isReversed;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__supplyCurrentLimit New supply current limit value @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetSupplyCurrentLimit(double __IN__supplyCurrentLimit)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.supplyCurrentLimit = __IN__supplyCurrentLimit;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__openLoopRampRate New open loop ramp rate value @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetOpenLoopRampRate(double __IN__openLoopRampRate)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.openLoopRampRate = __IN__openLoopRampRate;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__enableFOC If true, field oriented control is enabled @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetenableFOC(bool __IN__enableFOC)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.enableFOC = __IN__enableFOC;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetDutyCycleCallback(double* __IN__dutyCycleCallback)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.dutyCycleCallback = __IN__dutyCycleCallback;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__velocityCallback New angular velocity callback pointer @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetVelocityCallback(double* __IN__velocityCallback)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.velocityCallback = __IN__velocityCallback;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__positionCallback New angular position callback pointer @return Modified copy of create info*/
        KrakenX60MotorCreateInfo SetPositionCallback(double* __IN__positionCallback)
        {
            KrakenX60MotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.positionCallback = __IN__positionCallback;
            return modifiedCreateInfo;
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
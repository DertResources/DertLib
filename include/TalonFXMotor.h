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
    /** TalonFX supported motor types enum */
    enum TalonFXMotorType
    {
        KrakenX60,
        KrakenX44,
        Falcon500
    };

    /** Create info for a TalonFX motor */
    struct TalonFXMotorCreateInfo
    {
        // Motor Properties
        int canID = -1;
        const char* canbusName = "unfilled";
        bool isReversed = false;
        double supplyCurrentLimit = 15;
        double openLoopRampPeriod = 0;
        bool enableFOC = true;
        TalonFXMotorType motorType = TalonFXMotorType::KrakenX60;
        // Simulink Port Pointers
        double* dutyCycleCallback = nullptr;
        double* velocityCallback = nullptr;
        double* positionCallback = nullptr;

        static TalonFXMotorCreateInfo getDefaultCreateInfo()
        {
            TalonFXMotorCreateInfo modifiedCreateInfo;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__canID New CAN ID value @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetCanID(int __IN__canID)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.canID = __IN__canID;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__canbusName New smart current limit value @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetCanbusName(const char* __IN__canbusName)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.canbusName = __IN__canbusName;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__isReversed New motor reversal value @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetIsReversed(bool __IN__isReversed)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.isReversed = __IN__isReversed;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__supplyCurrentLimit New supply current limit value @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetSupplyCurrentLimit(double __IN__supplyCurrentLimit)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.supplyCurrentLimit = __IN__supplyCurrentLimit;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__openLoopRampPeriod New open loop ramp period value @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetOpenLoopRampPeriod(double __IN__openLoopRampPeriod)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.openLoopRampPeriod = __IN__openLoopRampPeriod;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__enableFOC If true, field oriented control is enabled @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetEnableFOC(bool __IN__enableFOC)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.enableFOC = __IN__enableFOC;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__motorType Type of motor this controller is plugged into @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetMotorType(TalonFXMotorType __IN__motorType)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.motorType = __IN__motorType;
            return modifiedCreateInfo;
        }

        /** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetDutyCycleCallback(double* __IN__dutyCycleCallback)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.dutyCycleCallback = __IN__dutyCycleCallback;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__velocityCallback New angular velocity callback pointer @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetVelocityCallback(double* __IN__velocityCallback)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.velocityCallback = __IN__velocityCallback;
            return modifiedCreateInfo;
        }
        
        /** Modify Create Info @param __IN__positionCallback New angular position callback pointer @return Modified copy of create info*/
        TalonFXMotorCreateInfo SetPositionCallback(double* __IN__positionCallback)
        {
            TalonFXMotorCreateInfo modifiedCreateInfo = *this;
            modifiedCreateInfo.positionCallback = __IN__positionCallback;
            return modifiedCreateInfo;
        }
    }; 



    /** A single TalonFX motor */
    class TalonFXMotor
    {
    public:
        /** Constructor for the TalonFX motor */
        TalonFXMotor(TalonFXMotorCreateInfo createInfo);
        
        /** Disable move constructor */
        TalonFXMotor(TalonFXMotor&& other) noexcept;

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
        TalonFXMotorCreateInfo finalCreateInfo{};

    private:
        /** Motor controller object
         * @warning This object has its copy and move constructors disabled
         */
        ctre::phoenix6::hardware::TalonFX talonController;

        /** Motor control object */
        ctre::phoenix6::controls::DutyCycleOut dutyCycleControl;
    };
};
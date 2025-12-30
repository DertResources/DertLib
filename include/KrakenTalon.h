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
    struct KrakenTalonCreateInfo
    {
        // Motor
        int canID;
        const char* canbus;
        bool isReversed = false;
        double supplyCurrentLimit = 15;
        double openLoopRampPeriod = 0;
        bool enableFOC = true;
        // DC
        double* setDutyCycleCallback = nullptr;
        // Sensor
        double* getVelocityCallback = nullptr;
        double* getPositionCallback = nullptr;

        /** Modifies info in the create info, mainly used for create infos
         * that are similar and share values
         * @param oldCreateInfo old create info
         * @param canID Can ID
         * @param setDutyCycleCallback pointer to simulink duty cycle value  
         * (set to nullptr if not used)
         * @param getVelocityCallback pointer to simulink velocity value - 
         * (set to nullptr if not used)
         * @param getPositionCallback pointer to simulink position value - 
         * (set to nullptr if not used)
         * @return modified return info
         */
        static constexpr KrakenTalonCreateInfo modifyInfo(
            KrakenTalonCreateInfo oldCreateInfo,
            int canID,
            double* setDutyCycleCallback = nullptr,
            double* getVelocityCallback = nullptr,
            double* getPositionCallback = nullptr)
        {
            auto newCreateInfo = oldCreateInfo;
            newCreateInfo.canID = canID;
            newCreateInfo.setDutyCycleCallback = setDutyCycleCallback;
            newCreateInfo.getVelocityCallback = getVelocityCallback;
            newCreateInfo.getPositionCallback = getPositionCallback;
            return newCreateInfo;
        }
    };

    /** A single Kraken motor */
    class KrakenTalon
    {
    public:
        // CONSTRUCTORS
        /** constructor for the Kraken Talon */
        KrakenTalon(KrakenTalonCreateInfo createInfo);
        
        /** disable move constructor */
        KrakenTalon(KrakenTalon&& other) noexcept;

        // CALLBACKS
        /** Callback for getting position of motor */
        void getPositionCallback();
        
        /** Callback for getting velocity of motor */
        void getVelocityCallback();
        
        /** Callback for setting duty cycle of motor */
        void setDutyCycleCallback();

        // CONTROLS
        /** set duty cycle of motor */
        void setDutyCycle(double DC);

        /** set brake mode of motor */
        void setBrakeMode(bool isBrakeMode);

        /** stop motor */
        void stopMotor();

        /** Motor controller object
         * @warning this object has its copy and move constructors disabled
         */
        ctre::phoenix6::hardware::TalonFX talonController;
        /** Motor control object */
        ctre::phoenix6::controls::DutyCycleOut dutyCycleControl;

        /** Create info for this motor */
        KrakenTalonCreateInfo finalCreateInfo{};
    private:
        /** Not useful now, but was created because there used to be multiple constructors */
        void initalizeTalon(KrakenTalonCreateInfo createInfo);
    };
};
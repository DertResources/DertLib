#pragma once
//std
#include <optional>
#include <cstdint>
//rev
#include <rev/SparkMax.h>
#include <rev/config/SparkMaxConfig.h>
#include <rev/SparkRelativeEncoder.h>

namespace dlib {
    /** Create info for Neo Spark */
    struct NeoSparkCreateInfo
    {
        //motor
        int canID;
        bool isReversed = false;
        uint8_t smartCurrentLimit = 15;
        double secondaryCurrentLimit = 15;
        double openLoopRampRate = 0.00000001;
        //sensor
        double* setDutyCycleCallback = nullptr;
        double* getVelocityCallback = nullptr;
        double* getPositionCallback = nullptr;
        rev::spark::SparkMax::MotorType motorType = rev::spark::SparkMax::MotorType::kBrushless;

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
        static constexpr NeoSparkCreateInfo modifyInfo(
            NeoSparkCreateInfo oldCreateInfo,
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
    class NeoSpark
    {
    public:
        // CONSTRUCTORS
        /** constructor for the Kraken Talon */
        NeoSpark(NeoSparkCreateInfo createInfo);

        /** disable move constructor */
        NeoSpark(NeoSpark&& other) noexcept;

        // CALLBACKS
        /** Callback for getting position of motor */
        void getPositionCallback();

        /** Callback for getting velocity of motor */
        void getVelocityCallback();

        /** Callback for setting duty cycle of motor */
        void setDutyCycleCallback();

        // CONTROLS
        /** set brake mode of motor */
        void setBrakeMode(bool isBrakeMode);

        /** set duty cycle of motor */
        void setDutyCycle(double DC);

        /** stop motor */
        void stopMotor();
        
        /** Motor controller object
         * @warning this object has its copy and move constructors disabled
         */
        rev::spark::SparkMax sparkMax;

        /** Encode object 
         * @warning this object has its copy and move constructors disabled
         */
        std::optional<rev::spark::SparkRelativeEncoder> sparkRelEncoder;

        /** Create info for this motor */
        NeoSparkCreateInfo finalCreateInfo{};
    private:
        /** Not useful now, but was created because there used to be multiple constructors */
        void initalizeSpark(NeoSparkCreateInfo createInfo);
        
    };
};
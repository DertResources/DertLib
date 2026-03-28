#pragma once

// Std
#include <optional>
#include <cstdint>

// Frc
#include <frc/Alert.h>

// Rev
#include <rev/SparkMax.h>
#include <rev/config/SparkMaxConfig.h>
#include <rev/SparkRelativeEncoder.h>
#include <rev/SparkBase.h>

namespace dlib {
    
/** Create info for SparkMax motor */
struct SparkMaxMotorCreateInfo
{
    // Motor Properties
    int canID = -1;
    std::string displayName;
    bool isReversed = false;
    uint8_t smartCurrentLimit = 15;
    double secondaryCurrentLimit = 15;
    double openLoopRampRate = 0.00000001;
    // Simulink Port Pointers
    double* dutyCycleCallback = nullptr;
    double* velocityCallback = nullptr;
    double* positionCallback = nullptr;
    rev::spark::SparkMax::MotorType motorType = rev::spark::SparkMax::MotorType::kBrushless;

    static SparkMaxMotorCreateInfo getDefaultCreateInfo();

    /** Modify Create Info @param __IN__canID New CAN ID value @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetCanID(int __IN__canID);
   
    
    /** Modify Create Info @param __IN__isReversed New motor reversal value @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetIsReversed(bool __IN__isReversed);
    

    /** Modify Create Info @param __IN__smartCurrentLimit New smart current limit value @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetSmartCurrentLimit(uint8_t __IN__smartCurrentLimit);


    /** Modify Create Info @param __IN__secondaryCurrentLimit New secondary current limit value @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetSecondaryCurrentLimit(double __IN__secondaryCurrentLimit);


    /** Modify Create Info @param __IN__openLoopRampRate New open loop ramp rate value @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetOpenLoopRampRate(double __IN__openLoopRampRate);
  

    /** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetDutyCycleCallback(double* __IN__dutyCycleCallback);
  
    
    /** Modify Create Info @param __IN__velocityCallback New angular velocity callback pointer @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetVelocityCallback(double* __IN__velocityCallback);
  
    
    /** Modify Create Info @param __IN__positionCallback New angular position callback pointer @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetPositionCallback(double* __IN__positionCallback);
   
    
    /** Modify Create Info @param __IN__motorType New motor type value @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetMotorType(rev::spark::SparkMax::MotorType __IN__motorType);

    /** Modify Create Info @param __IN__displayName New name of motor for alerts @return Modified copy of create info*/
    SparkMaxMotorCreateInfo SetDisplayName(std::string __IN__displayName);


    
};

/** A single SparkMax motor */
class SparkMaxMotor
{
public:
    /** Constructor for the SparkMax motor */
    SparkMaxMotor(SparkMaxMotorCreateInfo createInfo);

    /** Disable move constructor */
    SparkMaxMotor(SparkMaxMotor&& other) noexcept;

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

    /** Infers if the motor is connected based on errors */
    void UpdateCANConnectionAlert();

    /** Create info for this motor */
    SparkMaxMotorCreateInfo finalCreateInfo{};

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

    /** CAN connection warning */
    frc::Alert disconectedCANAlert;
    
    /** Group name for all SparkMax CAN Alerts */
    static constexpr std::string_view alertGroupName = "CAN Connection Alerts";
};

};
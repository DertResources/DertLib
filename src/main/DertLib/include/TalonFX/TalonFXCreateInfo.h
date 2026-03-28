#pragma once

#include <variant>
#include <cstdint>
#include <string>
#include <ctre/phoenix6/CANBus.hpp>
#include <units/frequency.h>
#include <units/angular_acceleration.h>

namespace dlib
{

namespace TalonFX
{


using AngularAccel = units::angular_acceleration::turns_per_second_squared_t;
using Hz = units::frequency::hertz_t;

/** TalonFX supported motor types enum */
enum MotorType
{
    KrakenX60,
    KrakenX44,
    Falcon500
};

/** Create info for a TalonFX motor */
struct BaseMotorCreateInfo
{
    // Motor Properties
    int canID;
    ctre::phoenix6::CANBus canbus;
    std::string displayName = "untitled controller";
    bool isReversed = false;
    double supplyCurrentLimit = 15;
    double openLoopRampPeriod = 0;
    bool enableFOC = true;
    MotorType motorType = MotorType::KrakenX60;

    // Simulink Port Pointers
    double* velocityCallback = nullptr;
    double* positionCallback = nullptr;

    /** Modify Create Info @param __IN__canID New CAN ID value @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetCanID(const int& __IN__canID);

    /** Modify Create Info @param __IN__canbus New canbus value @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetCanbus(const ctre::phoenix6::CANBus& __IN__canbus);

    /** Modify Create Info @param __IN__isReversed New motor reversal value @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetIsReversed(const bool& __IN__isReversed);

    /** Modify Create Info @param __IN__supplyCurrentLimit New supply current limit value @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetSupplyCurrentLimit(const double& __IN__supplyCurrentLimit);

    /** Modify Create Info @param __IN__openLoopRampPeriod New open loop ramp period value @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetOpenLoopRampPeriod(const double& __IN__openLoopRampPeriod);

    /** Modify Create Info @param __IN__enableFOC If true, field oriented control is enabled @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetEnableFOC(const bool& __IN__enableFOC);

    /** Modify Create Info @param __IN__motorType Type of motor this controller is plugged into @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetMotorType(const MotorType& __IN__motorType);

    /** Modify Create Info @param __IN__velocityCallback New angular velocity callback pointer @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetVelocityCallback( double*& __IN__velocityCallback);

    /** Modify Create Info @param __IN__positionCallback New angular position callback pointer @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetPositionCallback( double*& __IN__positionCallback);

    /** Modify Create Info @param __IN__displayName New display name for Alerts @return Modified copy of create info*/
    inline BaseMotorCreateInfo& SetDisplayName(const std::string& __IN__displayName);
}; 

struct DutyCycleCreateInfo : public BaseMotorCreateInfo
{
    double* dutyCycleCallback = nullptr;

    /** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
    DutyCycleCreateInfo& SetDutyCycleCallback(double*& __IN__dutyCycleCallback);
    
    inline DutyCycleCreateInfo Clone() {return *this;};

};

struct VelocityCreateInfo : public BaseMotorCreateInfo {
    double* P_Gain  = nullptr;
    double* I_Gain  = nullptr;
    double* D_Gain  = nullptr;
    double* FF = nullptr;
    //min = 20, max = 1000
    Hz updateFrequency = Hz(20);
    bool useTimeSync = false;
    int slot_PID = 0; //0-2
    bool updatePID = false;
    AngularAccel acceleration = AngularAccel(0);
    inline VelocityCreateInfo& SetPGain(double* __IN__P_Gain)             {P_Gain       = __IN__P_Gain                     ; return *this;}
    inline VelocityCreateInfo& SetIGain(double* __IN__I_Gain)             {I_Gain       = __IN__I_Gain                     ; return *this;}
    inline VelocityCreateInfo& SetDGain(double* __IN__D_Gain)             {D_Gain       = __IN__D_Gain                     ; return *this;}
    inline VelocityCreateInfo& SetFF(double* __IN__FF)           {FF      = __IN__FF                    ; return *this;}
    inline VelocityCreateInfo& SetUpdateFrequency(int __IN__updateFrequency) { updateFrequency = Hz(__IN__updateFrequency) ; return *this;}
    inline VelocityCreateInfo& SetUseTimeSync(bool __IN__useTimeSync)     {useTimeSync  = __IN__useTimeSync                ; return *this;}
    inline VelocityCreateInfo& SetupdatePID(double* __IN__updatePID)     {updatePID  = __IN__updatePID                ; return *this;}
    // Not currently needed, just disable this
    //inline VelocityCreateInfo& SetSlotPID(int __IN__slot_PID)             {slot_PID     = __IN__slot_PID                   ; return *this;}
    inline VelocityCreateInfo& SetAcceleration(double __IN__acceleration) {acceleration = AngularAccel(__IN__acceleration) ; return *this;}

    inline VelocityCreateInfo Clone() {return *this;};
};

struct FollowerCreateInfo : public BaseMotorCreateInfo {
    int leaderID = 0;
    bool isStrict = true;
    Hz updateFrequency = Hz(20);
    inline FollowerCreateInfo& SetLeaderID(int __IN__leaderID)               { leaderID = __IN__leaderID                        ; return *this;}
    inline FollowerCreateInfo& SetIsStrict(bool __IN__isStrict)              { isStrict = __IN__isStrict                            ; return *this;}
    inline FollowerCreateInfo& SetUpdateFrequency(int __IN__updateFrequency) { updateFrequency = Hz(__IN__updateFrequency) ; return *this;}
    inline FollowerCreateInfo Clone() {return *this;};

};

typedef enum : uint8_t {
    DUTYCYCLE, 
    VELOCITY,
    FOLLOWER,
    UNKNOWN
} MotorCreateInfoTypes;

typedef std::variant<DutyCycleCreateInfo, VelocityCreateInfo, FollowerCreateInfo> MotorCreateInfo;

inline DutyCycleCreateInfo GetNewDutyCycleCreateInfo() { auto a =  DutyCycleCreateInfo(); return a;}
inline VelocityCreateInfo  GetNewVelocityCreateInfo()  { auto a =  VelocityCreateInfo();  return a;}
inline FollowerCreateInfo  GetNewFollowerCreateInfo()  { auto a =  FollowerCreateInfo();  return a;}

}; //namespace TalonFX

}; //namespace dlib
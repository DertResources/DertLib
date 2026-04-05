#pragma once

#include <variant>
#include <string>
#include <variant>
#include "ctre/phoenix6/CANBus.hpp"
#include <units/frequency.h>
#include <units/angular_acceleration.h>
#include "../../include/TalonFX/TalonFXPidHandler.h"

namespace dlib::TalonFX
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
template <class Derived>
struct BaseMotorCreateInfo
{
protected:
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


    template<class COC, class CIC>
    friend class BaseTalonFXMotor;
public:
    inline Derived& SetCanID(const int& _canID)
        { this->canID = _canID; return static_cast<Derived&>(*this); }
    inline Derived& SetCanbus(const ctre::phoenix6::CANBus& _canbus)
        { this->canbus = _canbus; return static_cast<Derived&>(*this); }
    inline Derived& SetIsReversed(const bool& _isReversed)
        { this->isReversed = _isReversed; return static_cast<Derived&>(*this); }
    inline Derived& SetSupplyCurrentLimit(const double& _supplyCurrentLimit)
        { this->supplyCurrentLimit = _supplyCurrentLimit; return static_cast<Derived&>(*this); }
    inline Derived& SetOpenLoopRampPeriod(const double& _openLoopRampPeriod)
        { this->openLoopRampPeriod = _openLoopRampPeriod; return static_cast<Derived&>(*this); }
    inline Derived& SetEnableFOC(const bool& _enableFOC)
        { this->enableFOC = _enableFOC; return static_cast<Derived&>(*this); }
    inline Derived& SetMotorType(const MotorType& _motorType)
        { this->motorType = _motorType; return static_cast<Derived&>(*this); }
    inline Derived& SetVelocityCallback( double* _velocityCallback)
        { this->velocityCallback = _velocityCallback; return static_cast<Derived&>(*this); }
    inline Derived& SetPositionCallback( double* _positionCallback)
        { this->positionCallback = _positionCallback; return static_cast<Derived&>(*this); }
    inline Derived& SetDisplayName(const std::string& _displayName)
        { this->displayName = _displayName; return static_cast<Derived&>(*this); }
    inline Derived Clone() {return static_cast<Derived&>(*this);};
}; 

struct DutyCycleCreateInfo : public BaseMotorCreateInfo<DutyCycleCreateInfo>
{
private:
    double* dutyCycleCallback = nullptr;
    friend class DutyCycleControl;
public:
    inline DutyCycleCreateInfo& SetDutyCycleCallback(double* _dutyCycleCallback)
        { this->dutyCycleCallback = _dutyCycleCallback; return *this; }
};

struct VelocityCreateInfo : public BaseMotorCreateInfo<VelocityCreateInfo>,
                            public PidCreateInfo<VelocityCreateInfo>
{
private:
    double* targetVelocity = nullptr;
    double* FF = nullptr;
    Hz updateFrequency = Hz(20); //min = 20, max = 1000
    bool useTimeSync = false;
    AngularAccel acceleration = AngularAccel(0);
    friend class VelocityControl;
public:
    inline VelocityCreateInfo& SetTargetVelocity(double* _targetVelocity)
        {targetVelocity  = _targetVelocity; return *this;}
    inline VelocityCreateInfo& SetFF(double* _FF)                        
        {FF = _FF; return *this;}
    inline VelocityCreateInfo& SetUpdateFrequency(int _updateFrequency)
        {updateFrequency = Hz(std::clamp(_updateFrequency,20,1000)) ; return *this;}
    inline VelocityCreateInfo& SetUseTimeSync(bool _useTimeSync)
        {useTimeSync  = _useTimeSync                ; return *this;}
    inline VelocityCreateInfo& SetAcceleration(double _acceleration)
        {acceleration = AngularAccel(_acceleration) ; return *this;}
};

struct FollowerCreateInfo : public BaseMotorCreateInfo<FollowerCreateInfo> {
private:
    int leaderID = 0;
    Hz updateFrequency = Hz(20);
    friend class FollowerControl;
public:
    inline FollowerCreateInfo& SetLeaderID(int _leaderID)              
        { leaderID = _leaderID; return *this;}
    inline FollowerCreateInfo& SetUpdateFrequency(int _updateFrequency)
        { updateFrequency = Hz(_updateFrequency) ; return *this;}
};

typedef std::variant<DutyCycleCreateInfo, VelocityCreateInfo, FollowerCreateInfo> InnerType;


template <class T>
struct MotorCreateInfo
{
public:
    MotorCreateInfo(T info) : inner(std::move(info)) {}
    T Clone() {
        return std::get<T>(inner).Clone();
    }
private:
    std::variant<DutyCycleCreateInfo, VelocityCreateInfo, FollowerCreateInfo> inner;
    friend class TalonFXMotorGroup;
};

inline DutyCycleCreateInfo GetNewDutyCycleCreateInfo() { auto a =  DutyCycleCreateInfo(); return a;}
inline VelocityCreateInfo  GetNewVelocityCreateInfo()  { auto a =  VelocityCreateInfo();  return a;}
inline FollowerCreateInfo  GetNewFollowerCreateInfo()  { auto a =  FollowerCreateInfo();  return a;}

}; //namespace dlib::TalonFX
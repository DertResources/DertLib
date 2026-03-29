#include "../../include/TalonFX/TalonFXControlTypes.h"

namespace dlib::TalonFX 
{

// Duty Cycle Control

DutyCycleControl::DutyCycleControl(TalonFX::DutyCycleCreateInfo createInfo)
: ControlCreateInfo{createInfo} 
, ControlObj(0)
{
    ControlObj.WithEnableFOC(createInfo.enableFOC);
}

void DutyCycleControl::DeviceControlConfigure(Configurator* talonConfigurator) {}

void DutyCycleControl::SetBrakeMode(bool &isBrakeMode) 
{
    ControlObj.WithOverrideBrakeDurNeutral(isBrakeMode);
}

DCControlObj DutyCycleControl::GetControl(bool& selfDestruct)
{
    selfDestruct = false;
    ControlObj.WithOutput(*ControlCreateInfo.dutyCycleCallback);
    return ControlObj;
}

// Velocity Control

VelocityControl::VelocityControl(TalonFX::VelocityCreateInfo createInfo)
: ControlCreateInfo{createInfo}
, ControlObj(AngularVel(0))
, isFFEnabled{ControlCreateInfo.FF != nullptr}
{
    ControlObj.WithEnableFOC(ControlCreateInfo.enableFOC);
    ControlObj.WithAcceleration(ControlCreateInfo.acceleration);
    ControlObj.WithSlot(std::clamp(ControlCreateInfo.slot_PID,0,2));
    ControlObj.WithUpdateFreqHz(std::clamp(ControlCreateInfo.updateFrequency,Hz(20),Hz(1000)));
    ControlObj.WithUseTimesync(ControlCreateInfo.useTimeSync);

    if(isFFEnabled)
    {
        ControlObj.WithFeedForward(*ControlCreateInfo.FF);    
    }

}

void VelocityControl::DeviceControlConfigure(Configurator* talonConfigurator)
{
    slot0.WithKP(*ControlCreateInfo.P_Gain);
    slot0.WithKI(*ControlCreateInfo.I_Gain);
    slot0.WithKD(*ControlCreateInfo.D_Gain);
    talonConfigurator->Apply(slot0);
    this->talonConfigurator = talonConfigurator;
}

void VelocityControl::SetBrakeMode(bool &isBrakeMode)
{
    ControlObj.WithOverrideBrakeDurNeutral(isBrakeMode);
}

VelControlObj VelocityControl::GetControl(bool& selfDestruct)
{
    selfDestruct = false;
    if(ControlCreateInfo.updatePID)
    {
        slot0.WithKP(*ControlCreateInfo.P_Gain);
        slot0.WithKI(*ControlCreateInfo.I_Gain);
        slot0.WithKD(*ControlCreateInfo.D_Gain);
        this->talonConfigurator->Apply(slot0);    
    }
    if(isFFEnabled)
    {
        ControlObj.WithFeedForward(*ControlCreateInfo.FF);    
    }
    ControlObj.WithVelocity(AngularVel(*ControlCreateInfo.targetVelocity/60));
    return ControlObj;
}

// Follower Control

FollowerControl::FollowerControl(TalonFX::FollowerCreateInfo createInfo)
: ControlCreateInfo{createInfo}
, ControlObj(ControlCreateInfo.leaderID)
{
    ControlObj.WithUpdateFreqHz(ControlCreateInfo.updateFrequency);
}

void FollowerControl::DeviceControlConfigure(Configurator* talonConfigurator) {}

// No op, Follower follows break mode of leader
void FollowerControl::SetBrakeMode(bool &isBrakeMode) {}

FollowControlObj FollowerControl::GetControl(bool& selfDestruct)
{
    selfDestruct = true;
    return ControlObj;
}

}; //namespace dlib::TalonFX
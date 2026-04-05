#include "../../include/TalonFX/TalonFXMotorGroup.h"

namespace dlib::TalonFX 
{

// Duty Cycle Control // // // // // // // // // // // // // // // // // // //

DutyCycleControl::DutyCycleControl(TalonFX::DutyCycleCreateInfo createInfo)
: BaseTalonFXMotor(createInfo, 0)
{
    ControlObj.WithEnableFOC(createInfo.enableFOC);
}

void DutyCycleControl::UpdateControl()
{
    ControlObj.WithOutput(*savedCreateInfo.dutyCycleCallback);
}

// Velocity Control // // // // // // // // // // // // // // // // // // //

VelocityControl::VelocityControl(TalonFX::VelocityCreateInfo createInfo)
: BaseTalonFXMotor(createInfo, AngularVel(0))
, PidHandler(BaseTalonFXMotor::talonConfigurator,
             savedCreateInfo.slotList,
             savedCreateInfo.selectedSlot,
             ControlObj)
{
    isFFEnabled = savedCreateInfo.FF != nullptr;
    ControlObj.WithEnableFOC(savedCreateInfo.enableFOC);
    ControlObj.WithAcceleration(savedCreateInfo.acceleration);
    ControlObj.WithUpdateFreqHz(savedCreateInfo.updateFrequency);
    ControlObj.WithUseTimesync(savedCreateInfo.useTimeSync);


    if(isFFEnabled)
        ControlObj.WithFeedForward(*savedCreateInfo.FF);
}

void VelocityControl::UpdateControl()
{
    PollPidUpdate();
    
    if(isFFEnabled) ControlObj.WithFeedForward(*savedCreateInfo.FF);   

    ControlObj.WithVelocity(AngularVel(*savedCreateInfo.targetVelocity/60));
}

// Follower Control // // // // // // // // // // // // // // // // // // //

FollowerControl::FollowerControl(TalonFX::FollowerCreateInfo createInfo)
: BaseTalonFXMotor(createInfo, createInfo.leaderID)
{
    activeControlLoop = false;
    ControlObj.WithUpdateFreqHz(savedCreateInfo.updateFrequency);
}
// no op
void FollowerControl::UpdateControl() {}

}; //namespace dlib::TalonFX
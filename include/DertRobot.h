#pragma once

#include "../include/SparkMaxMotorGroup.h"
#include "../include/TalonFXMotorGroup.h"
#include "../include/HighFrequencyComponent.h"
#include "../include/Component.h"

#include <frc/TimedRobot.h>
#include <tuple>
namespace dlib
{

template <typename... Components>
class DertRobot : public frc::TimedRobot
{
  public:
  DertRobot();
  /** Runs once when robot turns on */
  void RobotInit()      override;
  /** The following code runs once when its mode is enabledd */
  void AutonomousInit() override;
  void TeleopInit()     override;
  void DisabledInit()   override;
  void TestInit()       override;
  void SimulationInit() override;

  /** Runs every 20ms regardless of what mode the robot is in */
  void RobotPeriodic()      override;
  void AutonomousPeriodic() override {};
  void TeleopPeriodic()     override {};
  void DisabledPeriodic()   override {};
  void TestPeriodic()       override {};
  void SimulationPeriodic() override {};

private:
    /** Transfer data from Odometry outputs to Robot_Control inputs */
    virtual void High_Frequency_to_Low_Frequency_Transfer() = 0;
    
    /** Puts all inputs from sensors and HIDs into Simulink */
    void PreStep() {for(auto component : dlib::Component::allCreatedComponents) component->PreStepCallback();}

    /** Takes outputs from simulink and pushes their commands to hardware */
    void PostStep() {for(auto component : dlib::Component::allCreatedComponents) component->PostStepCallback();}

    void HighFrequencyPreStep() {for(auto component : dlib::HighFrequencyComponent::allCreatedHighFrequencyComponents) component->HighFrequencyPreStepCallback();};

    void HighFrequencyPostStep() {for(auto component : dlib::HighFrequencyComponent::allCreatedHighFrequencyComponents) component->HighFrequencyPostStepCallback();};

    virtual void HighFrequencyStep() = 0;
    virtual void Step() = 0;
    
    typename std::tuple<Components...> ComponentArray;
};

};

#include "../cpp/DertRobot.imp"
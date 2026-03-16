#pragma once
#include <frc/TimedRobot.h>
#include <tuple>
namespace dlib
{

template <typename... Components>
class DertRobot : public frc::TimedRobot
{
  DertRobot();
public:
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
    void Odometry_to_Robot_Control_Transfer();
    
    /** Puts all inputs from sensors and HIDs into Simulink */
    void PreStep() {for(auto component : dlib::Component::allCreatedComponents) component->PreStepCallback();}

    /** Takes outputs from simulink and pushes their commands to hardware */
    void PostStep() {for(auto component : dlib::Component::allCreatedComponents) component->PostStepCallback();}

    void HighFrequencyPreStep() {for(auto component : dlib::HighFrequencyComponent::allCreatedHighFrequencyComponents) component->HighFrequencyPreStepCallback();};

    void HighFrequencyPostStep() {for(auto component : dlib::HighFrequencyComponent::allCreatedHighFrequencyComponents) component->HighFrequencyPreStepCallback();};

    std::tuple<Components> ComponentArray;
  /*
   * Below are the instances of the components used by the robot
   * Everything here should be direct hardware control, only
   * functions that manipulate global variables declared by Simulink
   * are exceptions to this rule.
   */

};

};
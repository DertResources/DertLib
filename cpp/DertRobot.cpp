#include "../include/DertRobot.h"

dib::DertRobot::DertRobot()
: ComponentArray{Components{}...}
{}

void dlib::DertRobot::RobotInit()
{   
    Robot_Control_U.GameState = -1;

    Robot_Control_initialize();
    Odometry_initialize();

    AddPeriodic([&](){
    static size_t HighFrequencyCount = 0;
    if(HighFrequencyCount != 0)
    {
      HighFrequencyPreStep();
      Odometry_step();
      HighFrequencyPostStep();
    }
    if(HighFrequencyCount == 2)
    {
      for (auto group : dlib::TalonFXMotorGroup::allTalonFXMotorGroups)
      {
        group->UpdateMotorCANConnectionAlerts();
      }
      
      for (auto group : dlib::SparkMaxMotorGroup::allSparkMaxMotorGroups)
      {
        group->UpdateMotorCANConnectionAlerts();
      }
    }
    if(HighFrequencyCount == 4)
      HighFrequencyCount = 0;
    else
      HighFrequencyCount++;
  },
  4_ms,
  0_ms);
}

void dlib::DertRobot::DisabledInit()   {Robot_Control_U.GameState = 0;}
void dlib::DertRobot::AutonomousInit() {Robot_Control_U.GameState = 1;}
void dlib::DertRobot::TeleopInit()     {Robot_Control_U.GameState = 2;}
void dlib::DertRobot::TestInit()       {Robot_Control_U.GameState = 3;}
void dlib::DertRobot::SimulationInit() {Robot_Control_U.GameState = 4;}

void dlib::DertRobot::RobotPeriodic() 
{  
  HighFrequencyPreStep();
  Odometry_step();      //Step the model
  HighFrequencyPostStep();

  Odometry_to_Robot_Control_Transfer();

  PreStep();
  Robot_Control_step(); //Step the model
  PostStep();
}
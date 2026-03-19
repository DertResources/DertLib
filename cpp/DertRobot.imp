#pragma once
#include <tuple>
template <typename... Components>
dlib::DertRobot<Components...>::DertRobot() {}

template <typename... Components>
void dlib::DertRobot<Components...>::RobotInit()
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
template <typename... Components>
void dlib::DertRobot<Components...>::DisabledInit()   {Robot_Control_U.GameState = 0;}

template <typename... Components>
void dlib::DertRobot<Components...>::AutonomousInit() {Robot_Control_U.GameState = 1;}

template <typename... Components>
void dlib::DertRobot<Components...>::TeleopInit()     {Robot_Control_U.GameState = 2;}

template <typename... Components>
void dlib::DertRobot<Components...>::TestInit()       {Robot_Control_U.GameState = 3;}

template <typename... Components>
void dlib::DertRobot<Components...>::SimulationInit() {Robot_Control_U.GameState = 4;}

template <typename... Components>
void dlib::DertRobot<Components...>::RobotPeriodic() 
{  
  HighFrequencyPreStep();
  HighFrequencyStep();      //Step the model
  HighFrequencyPostStep();

  High_Frequency_to_Low_Frequency_Transfer();

  PreStep();
  Step(); //Step the model
  PostStep();
}
#include "../../include/TalonFX/TalonFXMotorGroup.h"

namespace dlib::TalonFX
{

std::vector<TalonFXMotorGroup*> TalonFXMotorGroup::allTalonFXMotorGroups;

/** Constructor for the TalonFX motor group 
 * @param createInfos Array of TalonFX motor create infos
 */
TalonFXMotorGroup::TalonFXMotorGroup(std::initializer_list<TalonFX::MotorCreateInfo> createInfos)
{
    allTalonFXMotorGroups.push_back(this);


    for(auto createInfo : createInfos)
    {
        std::visit(
            [&](auto& info)
            {
                using T = std::decay_t<decltype(info)>;

                if constexpr (std::is_same_v<T, DutyCycleCreateInfo>)
                    motorSet.push_back(std::make_unique<MotorVarient>(
                    std::in_place_type<TalonFXMotor<DutyCycleControl>>, createInfo));
                else if constexpr (std::is_same_v<T, VelocityCreateInfo>)
                    motorSet.push_back(std::make_unique<MotorVarient>(
                    std::in_place_type<TalonFXMotor<VelocityControl>>, createInfo));
                else
                    motorSet.push_back(std::make_unique<MotorVarient>(
                    std::in_place_type<TalonFXMotor<FollowerControl>>, createInfo));
            }
            ,createInfo 
        );

        std::visit(
            overloads{
                [&](TalonFXMotor<DutyCycleControl>& obj) {AddCallbacks<DutyCycleControl>(obj);},
                [&](TalonFXMotor<VelocityControl>& obj)  {AddCallbacks<VelocityControl>(obj); },
                [&](TalonFXMotor<FollowerControl>& obj)  {AddCallbacks<FollowerControl>(obj); }
            }
            , *motorSet.back()
        );
    }
}

/** Stop all motors */
void TalonFXMotorGroup::Stop()
{
    for(auto& motor : motorSet)
    {
        std::visit(
            overloads{
                [&](TalonFXMotor<DutyCycleControl>& obj){obj.StopMotor();},
                [&](TalonFXMotor<VelocityControl>& obj) {obj.StopMotor();},
                [&](TalonFXMotor<FollowerControl>& obj) {obj.StopMotor();}
            },
            *motor
        );
    }
}

/** Set the brake mode when idle (coast / break) for all motors 
 * @param isBrakeMode The mode the motors will be in when command is zero.
 * true for break, false for coast
 */
void TalonFXMotorGroup::SetBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
    {
        std::visit(
            overloads{
                [&](TalonFXMotor<DutyCycleControl>& obj){obj.SetBrakeMode(isBrakeMode);},
                [&](TalonFXMotor<VelocityControl>& obj){obj.SetBrakeMode(isBrakeMode);},
                [&](TalonFXMotor<FollowerControl>& obj){obj.SetBrakeMode(isBrakeMode);}
            },
            *motor
        );
    }
}

/** Add all callbacks to the callback vectors
 * @param motor Reference to KrackenTalon object
 */
template <class T>
void TalonFXMotorGroup::AddCallbacks(TalonFX::TalonFXMotor<T>& motor)
{
    if(motor.IsAngularPositionRequested())
        positionCallbacks.push_back(std::bind(&TalonFXMotor<T>::SendPositionToSLCallback, &motor));
    if(motor.IsAngularVelocityRequested())
        velocityCallbacks.push_back(std::bind(&TalonFXMotor<T>::SendVelocityToSLCallback, &motor));
    ControlCallbacks.push_back(std::bind(&TalonFXMotor<T>::ControlLoop, &motor));
}

/** Pull positions and velocities from motor controllers and push them into simulink */
void TalonFXMotorGroup::SendSensorDataToSL()
{
    SendVelocityValuesToSL();
    SendPositionValuesToSL();
}

/** Pull commanded duty cycles from simulink outputs and push them to motor controllers */
void TalonFXMotorGroup::FetchMotorCommandsFromSL()
{
    for(auto func : ControlCallbacks)
        func();
}

/** Pull motor angular velocities from motor controllers and push them into simulink */
void TalonFXMotorGroup::SendVelocityValuesToSL()
{
    for(auto func : velocityCallbacks)
        func();
}

/** Pull motor angular positions from motor controllers and push them into simulink */
void TalonFXMotorGroup::SendPositionValuesToSL()
{
    for(auto func : positionCallbacks)
        func();
}

/** Update the warnings of CAN connections */
void TalonFXMotorGroup::UpdateMotorCANConnectionAlerts()
{
    for(auto& motor : motorSet)
    {
        std::visit(
            overloads{
                [](TalonFXMotor<DutyCycleControl>& obj){obj.UpdateCANConnectionAlert();},
                [](TalonFXMotor<VelocityControl>& obj){obj.UpdateCANConnectionAlert();},
                [](TalonFXMotor<FollowerControl>& obj){obj.UpdateCANConnectionAlert();}
            },
            *motor
        );
    }
}

};
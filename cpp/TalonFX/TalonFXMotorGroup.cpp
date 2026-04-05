#include "../../include/TalonFX/TalonFXMotorGroup.h"
#include "../../include/TalonFX/TalonFXPidHandler.h"
#include <memory>
#include <variant>

namespace dlib::TalonFX
{

std::vector<TalonFXMotorGroup*> TalonFXMotorGroup::allTalonFXMotorGroups;

/** Constructor for the TalonFX motor group 
 * @param createInfos Array of TalonFX motor create infos
 */
TalonFXMotorGroup::TalonFXMotorGroup(std::initializer_list<std::variant<
        MotorCreateInfo<DutyCycleCreateInfo>,
        MotorCreateInfo<VelocityCreateInfo>,
        MotorCreateInfo<FollowerCreateInfo>
        >>
         createInfos)
{
    allTalonFXMotorGroups.push_back(this);

    
    
    for(auto createInfo : createInfos)
    {
        InnerType inner = std::visit<InnerType>([](auto& info) {
            return info.inner;
        }, createInfo); 

        std::visit([&](auto& info)
        {
            using T = std::decay_t<decltype(info)>;
            using ControlType = typename ControlTypeFrom<T>::type;
            motorSet.push_back(std::make_unique<TalonFXMotor>(
                std::in_place_type<ControlType>, info));

            AddCallbacks<ControlType>(std::get<ControlType>(*motorSet.back()));
        },
        inner);
    }
}

/** Stop all motors */
void TalonFXMotorGroup::Stop()
{
    for(auto& motor : motorSet)
        std::visit([](auto& obj){obj.StopMotor();}, *motor );
}

/** Set the brake mode when idle (coast / break) for all motors 
 * @param isBrakeMode The mode the motors will be in when command is zero.
 * true for break, false for coast
 */
void TalonFXMotorGroup::SetBrakeModeWhenIdle(bool isBrakeMode)
{
    for(auto& motor : motorSet)
        std::visit([&](auto& obj){obj.SetBrakeMode(isBrakeMode);}, *motor );
}

/** Add all callbacks to the callback vectors
 * @param motor Reference to KrackenTalon object
 */
template <ControlTypeClass T>
void TalonFXMotorGroup::AddCallbacks(T& motor)
{
    if(motor.IsAngularPositionRequested())
        positionCallbacks.push_back(std::bind(&T::SendPositionToSLCallback, &motor));
    if(motor.IsAngularVelocityRequested())
        velocityCallbacks.push_back(std::bind(&T::SendVelocityToSLCallback, &motor));
    if(motor.HasControlLoop())
        ControlCallbacks.push_back(std::bind(&T::ControlLoop, &motor));
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
        std::visit([](auto& obj){obj.UpdateCANConnectionAlert();}, *motor );
}

};
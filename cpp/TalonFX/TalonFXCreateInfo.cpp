#include "../../include/TalonFX/TalonFXCreateInfo.h"

namespace dlib
{

namespace TalonFX
{
    
/** Modify Create Info @param __IN__dutyCycleCallback New dutycycle callback pointer @return Modified copy of create info*/
DutyCycleCreateInfo& DutyCycleCreateInfo::SetDutyCycleCallback(double* __IN__dutyCycleCallback)
{ this->dutyCycleCallback = __IN__dutyCycleCallback; return *this; }


}; // namespace TalonFX

}; //namespace dlib
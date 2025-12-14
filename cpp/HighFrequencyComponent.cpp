#include "../include/HighFrequencyComponent.h"

std::vector<dlib::HighFrequencyComponent*> dlib::HighFrequencyComponent::AllCreatedHighFrequencyComponents; 

dlib::HighFrequencyComponent::HighFrequencyComponent()
{
    AllCreatedHighFrequencyComponents.push_back(this);
}

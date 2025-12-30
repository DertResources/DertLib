#include "../include/HighFrequencyComponent.h"

std::vector<dlib::HighFrequencyComponent*> dlib::HighFrequencyComponent::AllCreatedHighFrequencyComponents; 

/**
 * This constructor adds a pointer to the object owned by the children
 * of this class to the vector 'AllCreatedHighFrequencyComponents'
 */
dlib::HighFrequencyComponent::HighFrequencyComponent()
{
    AllCreatedHighFrequencyComponents.push_back(this);
}

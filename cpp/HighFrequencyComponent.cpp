#include "../include/HighFrequencyComponent.h"

std::vector<dlib::HighFrequencyComponent*> dlib::HighFrequencyComponent::allCreatedHighFrequencyComponents; 

/**
 * This constructor adds a pointer to the object owned by the children
 * of this class to the vector 'AllCreatedHighFrequencyComponents'
 */
dlib::HighFrequencyComponent::HighFrequencyComponent()
{
    allCreatedHighFrequencyComponents.push_back(this);
}

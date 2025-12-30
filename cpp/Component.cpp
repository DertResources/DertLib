#include "../include/Component.h"

std::vector<dlib::Component*> dlib::Component::AllCreatedComponents; 

/**
 * This constructor adds a pointer to the object owned by the children
 * of this class to the vector 'AllCreatedComponents'
 */
dlib::Component::Component()
{
    AllCreatedComponents.push_back(this);
}   
#include "../include/Component.h"

std::vector<dlib::Component*> dlib::Component::allCreatedComponents; 

/**
 * This constructor adds a pointer to the object owned by the children
 * of this class to the vector 'AllCreatedComponents'
 */
dlib::Component::Component()
{
    allCreatedComponents.push_back(this);
}   
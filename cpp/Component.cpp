#include "../include/Component.h"

std::vector<dlib::Component*> dlib::Component::AllCreatedComponents; 

dlib::Component::Component()
{
    AllCreatedComponents.push_back(this);
}   
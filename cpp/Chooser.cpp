#include "../include/Chooser.h"

/**
 * Manual chooser constructor, the user must grab results each loop
 * @param chooserName Title of the chooser
 * @param chooserMap Map of options in the chooser to the ints that are sent to simulink
 */
dlib::Chooser::Chooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap)
{
    InitChooser(ChooserName, chooserMap);
}

/**
 * Simulink based constructor, this will automatically send user selections to simulink
 * @param chooserName Title of the chooser
 * @param chooserMap Map of options in the chooser to the ints that are sent to simulink
 * @param simulinkPointer pointer to the simulink U port.
 */
dlib::Chooser::Chooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap, double* simulinkPointer)
{
    InitChooser(ChooserName, chooserMap);
    chooserSelectedKeyPointer = simulinkPointer;

    m_chooser.OnChange(
        [&](std::string currentSelection){
            *chooserSelectedKeyPointer = GetSelectedKey(currentSelection);
        }
    );
}

/**
 * Setup chooser graphic in elastic
 * @param chooserName Title of chooser in Elastic
 * @param chooserMap  Map of options in the chooser, and the output int for each
 */
void dlib::Chooser::InitChooser(std::string chooserName, std::vector<std::pair<std::string, int>> chooserMap)
{
    this->chooserMap = chooserMap;

    if(chooserMap.size() == 0)
        throw std::runtime_error("Chooser Length must be greater than zero");

    m_chooser.SetDefaultOption(chooserMap.at(0).first, chooserMap.at(0).first);

    for(size_t pos = 1; pos < chooserMap.size(); pos++)
        m_chooser.AddOption(chooserMap.at(pos).first, chooserMap.at(pos).first);

    frc::SmartDashboard::PutData(ChooserName, &m_chooser);
}

/**
 * Gets the int that matches the input key
 * @param currentSelection name of option in chooser, or the 'key'
 * @return the mapped integer for the string key 'currentSelection'
 * @warning the default value if the current selected is not found is -1
 */
double dlib::Chooser::GetSelectedKey(std::string currentSelection)
{
    for(size_t pos = 0; pos < chooserMap.size(); pos++)
    {
        std::pair<std::string, int> pairAtPos = chooserMap.at(pos);

        if(pairAtPos.first == currentSelection)
            return pairAtPos.second;
    }
    return -1;
}

/**
 * Gets the current user selected option and returns the int that maps to that
 */
double dlib::Chooser::GetSelectedKey()
{
    std::string currentSelection = m_chooser.GetSelected();
    return GetSelectedKey(currentSelection);
}
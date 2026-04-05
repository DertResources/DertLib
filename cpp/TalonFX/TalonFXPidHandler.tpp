#pragma once
// Just so intelisense can see the header and highlight correctly
#include "../../include/TalonFX/TalonFXPidHandler.h"

namespace dlib::TalonFX
{

template<PidControlType CtrlObj, class ControlCreateInfo>
PidHandler<CtrlObj, ControlCreateInfo>::PidHandler(
        ctre::phoenix6::configs::TalonFXConfigurator& _talonConfigurator,
        std::vector<SlotContainer<ControlCreateInfo>>& _slotConfigs,
        std::variant<int, double*> _selectedSlot,
        CtrlObj& _obj)
    : talonConfigurator{_talonConfigurator}
    , slotConfigs{_slotConfigs}
    , updatePidSlot(std::visit(overloads{
            [&](int i){_obj.WithSlot(i);  return false;},
            [&](double* i){slotUpdatePtr = i; return true;}
        },_selectedSlot))
    ,m_ctrlObj(_obj)
{
    for(SlotContainer<ControlCreateInfo> slot : slotConfigs)
    {
        // create slot configuration
        SlotConfigurators.emplace_back();

        // Set up live update mappings
        if(slot.updatePID)
        {
            liveUpdateMapping.emplace_back();
            liveUpdateMapping.back().first = SlotConfigurators.size()-1;
            if(slot.P_Gain.index() == 1)
                liveUpdateMapping.back().second.push_back(
                    [this](size_t idx){SetPGain(idx);});
            
            if(slot.I_Gain.index() == 1)
                liveUpdateMapping.back().second.push_back(
                    [this](size_t idx){SetIGain(idx);});

            if(slot.D_Gain.index() == 1)
                liveUpdateMapping.back().second.push_back(
                    [this](size_t idx){SetDGain(idx);});

            if(liveUpdateMapping.back().second.size() == 0)
                liveUpdateMapping.pop_back();
        }

        // first initalization
        SetPGain(SlotConfigurators.size()-1);
        SetIGain(SlotConfigurators.size()-1);
        SetDGain(SlotConfigurators.size()-1);

        int slotIdx = slot.slotIndex;
        if(slotIdx < 0 || slotIdx > 2)
        {
            fmt::println(stderr, "DLIB::ERROR: TalonFX Pid slots must be [0,2], you entered {}", slotIdx);
            continue;
        }
        SlotConfigurators.back().SlotNumber = slotIdx;
        talonConfigurator.Apply(SlotConfigurators.back());
    }
}

template<PidControlType CtrlObj, class ControlCreateInfo>
double PidHandler<CtrlObj, ControlCreateInfo>::resolveGain(std::variant<double, double*> gain)
    {
        return std::visit(overloads{
            [](double  gain){return gain;},
            [](double* gain){return *gain;},
            },
            gain
        );
    }

template<PidControlType CtrlObj, class ControlCreateInfo>
void PidHandler<CtrlObj, ControlCreateInfo>::SetPGain(size_t configuratorIndex)
{
    SlotConfigurators[configuratorIndex]
        .WithKP(resolveGain(slotConfigs[configuratorIndex].P_Gain));
}

template<PidControlType CtrlObj, class ControlCreateInfo>
void PidHandler<CtrlObj, ControlCreateInfo>::SetIGain(size_t configuratorIndex)
{
    SlotConfigurators[configuratorIndex]
        .WithKI(resolveGain(slotConfigs[configuratorIndex].I_Gain));
}

template<PidControlType CtrlObj, class ControlCreateInfo>
void PidHandler<CtrlObj, ControlCreateInfo>::SetDGain(size_t configuratorIndex)
{
    SlotConfigurators[configuratorIndex]
        .WithKD(resolveGain(slotConfigs[configuratorIndex].D_Gain));
}

template<PidControlType CtrlObj, class ControlCreateInfo>
void PidHandler<CtrlObj, ControlCreateInfo>::PollPidUpdate()
{
    if(updatePidSlot)
        m_ctrlObj.WithSlot(static_cast<int>(*slotUpdatePtr));

    for(std::pair<int, std::vector<std::function<void(size_t)>>>& slot : liveUpdateMapping)
    {
        if(*slotConfigs[slot.first].updatePID != 0)
        {
            for(std::function<void(size_t)>& updateGain : slot.second)
            {
                updateGain(slot.first);
            }
            talonConfigurator.Apply(SlotConfigurators[slot.first]);
        }
    }
}


template <class Derived>
PidCreateInfo<Derived>::PidCreateInfo(const PidCreateInfo& other)
{
    slotList = other.slotList;
    selectedSlot = other.selectedSlot;
    Derived* selfPtr = static_cast<Derived*>(this);
    for (SlotContainer<Derived>& S : slotList)
        S.parentPtr = selfPtr;
}

template <class Derived>
PidCreateInfo<Derived>::PidCreateInfo(PidCreateInfo&& other) noexcept
{
    slotList = std::move(other.slotList);   // steals the vector, leaves other.slotList empty
    selectedSlot = std::move(other.selectedSlot);
    Derived* selfPtr = static_cast<Derived*>(this);
    for (SlotContainer<Derived>& S : slotList)
        S.parentPtr = selfPtr;
}

template <class Derived>
PidCreateInfo<Derived>& PidCreateInfo<Derived>::operator=(const PidCreateInfo& other)
{
    if (this == &other) return *this;
    slotList = other.slotList;
    selectedSlot = other.selectedSlot;
    Derived* selfPtr = static_cast<Derived*>(this);
    for (SlotContainer<Derived>& S : slotList)
        S.parentPtr = selfPtr;
    return *this;
}

template <class Derived>
PidCreateInfo<Derived>& PidCreateInfo<Derived>::operator=(PidCreateInfo&& other) noexcept
{
    if (this == &other) return *this;
    slotList = std::move(other.slotList);
    selectedSlot = std::move(other.selectedSlot);
    Derived* selfPtr = static_cast<Derived*>(this);
    for (SlotContainer<Derived>& S : slotList)
        S.parentPtr = selfPtr;
    return *this;
}


}; //namespace: dlib::TalonFX

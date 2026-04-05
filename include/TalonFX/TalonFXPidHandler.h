#pragma  once

// Std
#include <stdexcept>
#include <variant>

#include <ctre/phoenix6/core/CoreTalonFX.hpp>
#include "ctre/phoenix6/configs/SlotConfigs.hpp"
#include "ctre/phoenix6/configs/Slot0Configs.hpp"
#include "ctre/phoenix6/configs/Slot1Configs.hpp"
#include "ctre/phoenix6/configs/Slot2Configs.hpp"
#include "fmt/core.h"
#include "fmt/base.h"
#include <functional>

namespace dlib::TalonFX {

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };

template<typename T>
concept PidControlType = requires(T t, int slotnum)
{
    t.WithSlot(slotnum);
};

template<class Derived> struct PidCreateInfo;
template<class Parent>  struct SlotContainer;

template<class Parent>
struct SlotContainer
{
private:
    Parent* parentPtr = nullptr;
    int slotIndex = 0;
    std::variant<double, double*> P_Gain  = static_cast<double>(0); // 0 if someone forgets to set it 
    std::variant<double, double*> I_Gain  = static_cast<double>(0); // 0 if someone forgets to set it
    std::variant<double, double*> D_Gain  = static_cast<double>(0); // 0 if someone forgets to set it
    double* updatePID = nullptr;
    template<PidControlType CO, class CI> friend class PidHandler;
    friend struct PidCreateInfo<Parent>;
public:
    inline SlotContainer& SetPGain(std::variant<double, double*> _P_Gain)
        {P_Gain = _P_Gain; return *this;}
    inline SlotContainer& SetIGain(std::variant<double, double*> _I_Gain)
        {I_Gain = _I_Gain; return *this;}
    inline SlotContainer& SetDGain(std::variant<double, double*> _D_Gain)
        {D_Gain = _D_Gain; return *this;}
    inline SlotContainer& SetupdatePID(double* _updatePID)
        {updatePID  = _updatePID; return *this;}
    inline Parent& ExitPidConfig()
        {return *parentPtr;}
    inline SlotContainer& EditPIDSlot(int slotNum)
        {return parentPtr->EditPIDSlot(slotNum);}
};

template <class Derived>
struct PidCreateInfo
{
protected:
    std::vector<dlib::TalonFX::SlotContainer<Derived>> slotList;
    std::variant<int, double*> selectedSlot;
    template<PidControlType CO, class CI> friend class PidHandler;
public:
    PidCreateInfo() = default;
    PidCreateInfo(const PidCreateInfo& other);
    PidCreateInfo(PidCreateInfo&& other) noexcept;
    PidCreateInfo<Derived>& operator=(const PidCreateInfo& other);
    PidCreateInfo<Derived>& operator=(PidCreateInfo&& other) noexcept;

    inline Derived& SetSelectedSlot(std::variant<int, double*> _selectedSlot)
        {selectedSlot =_selectedSlot; return static_cast<Derived&>(*this);}

    inline dlib::TalonFX::SlotContainer<Derived>& EditPIDSlot(int slotNum)
    {
        slotList.emplace_back();
        slotList.back().parentPtr = static_cast<Derived*>(this);
        slotList.back().slotIndex = slotNum;
        return slotList.back();
    }
};

template<PidControlType CtrlObj, class ControlCreateInfo>
class PidHandler
{
public:
    PidHandler(ctre::phoenix6::configs::TalonFXConfigurator& _talonConfigurator,
               std::vector<SlotContainer<ControlCreateInfo>>& _slotConfigs,
               std::variant<int, double*> _selectedSlot,
               CtrlObj& obj);

    double resolveGain(std::variant<double, double*> gain);

    void SetPGain(size_t configuratorIndex);
    void SetIGain(size_t configuratorIndex);
    void SetDGain(size_t configuratorIndex);

    void PollPidUpdate();
private:
    ctre::phoenix6::configs::TalonFXConfigurator& talonConfigurator;
    std::vector<SlotContainer<ControlCreateInfo>>& slotConfigs;
    std::vector<ctre::phoenix6::configs::SlotConfigs> SlotConfigurators;
    std::vector<std::pair<int, std::vector<std::function<void(size_t)>>>> liveUpdateMapping;
    const bool updatePidSlot;
    double* slotUpdatePtr = nullptr;
    CtrlObj& m_ctrlObj;
};




}; //namespace: dlib::TalonFX

#include "../../cpp/TalonFX/TalonFXPidHandler.tpp"
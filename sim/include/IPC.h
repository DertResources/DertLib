#pragma once
#include "../include/SerializeHelper.h"
#include "DataEntry.h"
#include "Device.h"
#include <vector>
#include <cstdint>
#include <functional>
#include <unordered_map>

namespace dlibsim
{

class IPC
{

typedef enum
{
    READ,
    WRITE

} MessageState;

public: 
    ByteVector byteBuffer;

    //    1   .   0   .   0     |    2026   April
    // (Major).(Minor).(Patch)  |   (Year).(Month)
    const uint8_t majorVersion = 1; // Major Breaking Changes
    const uint8_t minorVersion = 0; // Minor Changes
    const uint8_t patchVersion = 0; // Patch Versions
    const uint8_t releaseYear  = 26; // ex: 26 (2026)
    const uint8_t releaseMonth = 4; // ex: 4 (April)

    void ReadFailure        (); void WriteFailure        ();
    void ReadSuccess        (); void WriteSuccess        ();
    void ReadVersion        (); void WriteVersion        ();
    void ReadFatalError     (); void WriteFatalError     ();
    void ReadDeviceTypes    (); void WriteDeviceTypes    ();
    void ReadDeviceInstances(); void WriteDeviceInstances();
public:
    void WriteToBuffer();
    void ReadFromBuffer();
    void PrintVersion();

    static constexpr std::pair<uint16_t, void(*)(IPC&)> writeHandlers[] = 
    {
        { 0x0001, [](IPC& ipc){ipc.WriteFailure         ();} },
        { 0x0002, [](IPC& ipc){ipc.WriteSuccess         ();} },
        { 0x0003, [](IPC& ipc){ipc.WriteVersion         ();} },
        { 0x0004, [](IPC& ipc){ipc.WriteFatalError      ();} },
        { 0x0005, [](IPC& ipc){ipc.WriteDeviceTypes     ();} },
        { 0x0006, [](IPC& ipc){ipc.WriteDeviceInstances ();} }
    };
    static constexpr std::pair<uint16_t, void(*)(IPC&)> readHandlers[] =
    {
        { 0x0001, [](IPC& ipc){ipc.ReadFailure         ();} },
        { 0x0002, [](IPC& ipc){ipc.ReadSuccess         ();} },
        { 0x0003, [](IPC& ipc){ipc.ReadVersion         ();} },
        { 0x0004, [](IPC& ipc){ipc.ReadFatalError      ();} },
        { 0x0005, [](IPC& ipc){ipc.ReadDeviceTypes     ();} },
        { 0x0006, [](IPC& ipc){ipc.ReadDeviceInstances ();} }
    };

        
};

};
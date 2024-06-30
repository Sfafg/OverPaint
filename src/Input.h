#pragma once
#include <functional>
#include "Keys.h"
#include "VG/include/Flags.h"
class Input
{
public:
    enum class Device
    {
        Mouse,
        Keyboard,
        Pen
    };

    Input();
    static void SetPointerCallback(std::function<void(bool, int, int, float, Device)> callback);
    static void SetButtonCallback(std::function<void(Button, State, Device)> callback);
    static void SetKeyCallback(std::function<void(Key, State, Device)> callback);
    static void SetScrollCallback(std::function<void(int, int, Device)> callback);
    static vg::Flags<Mods> GetMods();
};
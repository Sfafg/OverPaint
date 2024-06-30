#undef UNICODE
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include "Input.h"

static std::function<void(bool, int, int, float, Input::Device)> pointerCallback;
static std::function<void(Button, State, Input::Device)> buttonCallback;
static std::function<void(Key, State, Input::Device)> keyCallback;
static std::function<void(int, int, Input::Device)> scrollCallback;
static vg::Flags<Mods> mods;
static bool isLMouseDown = false;

void Input::SetPointerCallback(std::function<void(bool, int, int, float, Input::Device)> callback)
{
    pointerCallback = callback;
}
void Input::SetButtonCallback(std::function<void(Button, State, Input::Device)> callback)
{
    buttonCallback = callback;
}
void Input::SetKeyCallback(std::function<void(Key, State, Input::Device)> callback)
{
    keyCallback = callback;
}
void Input::SetScrollCallback(std::function<void(int, int, Input::Device)> callback)
{
    scrollCallback = callback;
}

vg::Flags<Mods> Input::GetMods()
{
    return mods;
}


class Hook
{
    HHOOK hook;
public:
    Hook() :hook(NULL) {};
    Hook(HHOOK hook) :hook(hook)
    {}
    ~Hook()
    {
        if (hook == NULL) return;
        UnhookWindowsHookEx(hook);
    }
    Hook& operator=(const HHOOK& hook_) { hook = hook_; return *this; }
    operator HHOOK () { return hook; }
};

Hook keyboardHook;
Hook mouseHook;
Key VKToKey(int vk);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KEYHookCallback(int nCode, WPARAM downParam, LPARAM keyDataParam);
LRESULT CALLBACK MOUSEHookCallback(int nCode, WPARAM wP, LPARAM lP);
Input::Input()
{
    std::thread thread([]()
        {
            WNDCLASS wc = {};
            wc.lpfnWndProc = WindowProc;
            wc.hInstance = GetModuleHandle(NULL);
            wc.lpszClassName = "Dummy";
            RegisterClass(&wc);

            HWND window = CreateWindowEx(0, "Dummy", "Dummy", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
            RAWINPUTDEVICE Rid[1];
            Rid[0].usUsagePage = 0x0D;
            Rid[0].usUsage = 0x01;
            Rid[0].dwFlags = RIDEV_INPUTSINK;
            Rid[0].hwndTarget = window;
            RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
            Input::GetMods().SetState(Mods::CapsLock, GetKeyState(VK_CAPITAL));
            Input::GetMods().SetState(Mods::NumLock, GetKeyState(VK_NUMLOCK));

            if (!(keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KEYHookCallback, NULL, NULL)))
                std::cout << "Failed to install KeyboardHook hook.";

            if (!(mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MOUSEHookCallback, NULL, NULL)))
                std::cout << "Failed to install MouseHook hook.";

            MSG Msg;
            while (GetMessage(&Msg, NULL, 0, 0))
            {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
        });
    thread.detach();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INPUT:
    {
        UINT dataSize;
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
        std::vector<BYTE> buffer(dataSize);
        if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer.data(), &dataSize, sizeof(RAWINPUTHEADER)) != dataSize)
            return 0;

        RAWINPUT* rawInput = reinterpret_cast<RAWINPUT*>(buffer.data());
        if (rawInput->header.dwType == RIM_TYPEHID)
        {
            bool isDown = rawInput->data.hid.bRawData[1] == 3;
            int x = *(short*) &rawInput->data.hid.bRawData[2];
            int y = *(short*) &rawInput->data.hid.bRawData[4];
            float pressure = *(short*) &rawInput->data.hid.bRawData[6] / 8192.0f;

            int width = GetSystemMetrics(SM_CXFULLSCREEN);
            int height = GetSystemMetrics(SM_CYFULLSCREEN);
            height = 1080;
            pointerCallback(isDown, x / 32767.0f * width, y / 32767.0f * height, pressure, Input::Device::Pen);
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK KEYHookCallback(int nCode, WPARAM downParam, LPARAM keyDataParam)
{
    if (nCode < 0) return CallNextHookEx(keyboardHook, nCode, downParam, keyDataParam);

    KBDLLHOOKSTRUCT* keyData = (KBDLLHOOKSTRUCT*) keyDataParam;
    bool isPressed = downParam == WM_KEYDOWN || downParam == WM_SYSKEYDOWN;
    Key key = VKToKey(keyData->vkCode);

    switch (key)
    {
    case Key::LeftShift:
    case Key::RightShift:
        mods.SetState(Mods::Shift, isPressed);
        break;
    case Key::LeftControl:
    case Key::RightControl:
        mods.SetState(Mods::Control, isPressed);
        break;
    case Key::LeftAlt:
    case Key::RightAlt:
        mods.SetState(Mods::Alt, isPressed);
        break;
    case Key::CapsLock:
        mods.SetState(Mods::CapsLock, !mods.IsSet(Mods::CapsLock));
        break;
    case Key::NumLock:
        mods.SetState(Mods::NumLock, !mods.IsSet(Mods::NumLock));
        break;
    default:break;
    }

    if (keyCallback) keyCallback(key, (State) isPressed, Input::Device::Keyboard);
    return CallNextHookEx(keyboardHook, nCode, downParam, keyDataParam);
}
LRESULT CALLBACK MOUSEHookCallback(int nCode, WPARAM wP, LPARAM lP)
{
    if (nCode < 0) return CallNextHookEx(mouseHook, nCode, wP, lP);

    MSLLHOOKSTRUCT* keyData = (MSLLHOOKSTRUCT*) lP;
    bool isPressed = false;
    Button button = Button::None;
    if (WM_LBUTTONDOWN <= wP && wP <= WM_MBUTTONDBLCLK)
    {
        button = (Button) ((wP - WM_LBUTTONDOWN) / 3 + 1);
        isPressed = (wP - WM_LBUTTONDOWN) % 3 != 1;
    }
    else if (WM_XBUTTONDOWN <= wP && wP <= WM_XBUTTONDBLCLK)
    {
        button = (Button) (HIWORD(keyData->mouseData) + 3);
        isPressed = (wP - WM_XBUTTONDOWN) % 3 != 1;
    }
    if (button == Button::Left) isLMouseDown = isPressed;
    if (button != Button::None && buttonCallback) buttonCallback(button, (State) isPressed, Input::Device::Mouse);

    switch (wP)
    {
    case WM_MOUSEWHEEL:
        if (scrollCallback) scrollCallback(0, HIWORD(keyData->mouseData), Input::Device::Mouse);
        break;
    case WM_MOUSEHWHEEL:
        if (scrollCallback) scrollCallback(HIWORD(keyData->mouseData), 0, Input::Device::Mouse);
        break;
    case WM_MOUSEMOVE:
        if (pointerCallback) pointerCallback(isLMouseDown, keyData->pt.x, keyData->pt.y, 0.5f * isLMouseDown, Input::Device::Mouse);
        break;
    default:
        break;
    }

    return CallNextHookEx(mouseHook, nCode, wP, lP);
}


Key VKToKey(int vk)
{
    static std::map<int, Key> keyMappingTable = {
    { VK_SPACE, Key::Space },
    { VK_OEM_7, Key::Apostrophe },
    { VK_OEM_COMMA, Key::Comma },
    { VK_OEM_MINUS, Key::Minus },
    { VK_OEM_PERIOD, Key::Period },
    { VK_OEM_2, Key::Slash },
    {  '0', Key::Zero },
    { '1', Key::One },
    { '2', Key::Two },
    { '3', Key::Three },
    { '4', Key::Four },
    { '5', Key::Five },
    { '6', Key::Six },
    { '7', Key::Seven },
    { '8', Key::Eight },
    { '9', Key::Nine },
    { VK_OEM_1, Key::Semicolon },
    { VK_OEM_PLUS, Key::Equal },
    { 'A', Key::A },
    { 'B', Key::B },
    { 'C', Key::C },
    { 'D', Key::D },
    { 'E', Key::E },
    { 'F', Key::F },
    { 'G', Key::G },
    { 'H', Key::H },
    { 'I', Key::I },
    { 'J', Key::J },
    { 'K', Key::K },
    { 'L', Key::L },
    { 'M', Key::M },
    { 'N', Key::N },
    { 'O', Key::O },
    { 'P', Key::P },
    { 'Q', Key::Q },
    { 'R', Key::R },
    { 'S', Key::S },
    { 'T', Key::T },
    { 'U', Key::U },
    { 'V', Key::V },
    { 'W', Key::W },
    { 'X', Key::X },
    { 'Y', Key::Y },
    { 'Z', Key::Z },
    { VK_OEM_4, Key::LeftBracket },
    { VK_OEM_5, Key::Backslash },
    { VK_OEM_6, Key::RightBracket },
    { VK_OEM_3, Key::GraveAccent },
    { 0xE001, Key::World1 },
    { 0xE002, Key::World2 },
    { VK_ESCAPE, Key::Escape },
    { VK_RETURN, Key::Enter },
    { VK_TAB, Key::Tab },
    { VK_BACK, Key::Backspace },
    { VK_INSERT, Key::Insert },
    { VK_DELETE, Key::Delete },
    { VK_RIGHT, Key::Right },
    { VK_LEFT, Key::Left },
    { VK_DOWN, Key::Down },
    { VK_UP, Key::Up },
    { VK_PRIOR, Key::PageUp },
    { VK_NEXT, Key::PageDown },
    { VK_HOME, Key::Home },
    { VK_END, Key::End },
    { VK_CAPITAL, Key::CapsLock },
    { VK_SCROLL, Key::ScrollLock },
    { VK_NUMLOCK, Key::NumLock },
    { VK_SNAPSHOT, Key::PrintScreen },
    { VK_PAUSE, Key::Pause },
    { VK_F1, Key::F1 },
    { VK_F2, Key::F2 },
    { VK_F3, Key::F3 },
    { VK_F4, Key::F4 },
    { VK_F5, Key::F5 },
    { VK_F6, Key::F6 },
    { VK_F7, Key::F7 },
    { VK_F8, Key::F8 },
    { VK_F9, Key::F9 },
    { VK_F10, Key::F10 },
    { VK_F11, Key::F11 },
    { VK_F12, Key::F12 },
    { VK_F13, Key::F13 },
    { VK_F14, Key::F14 },
    { VK_F15, Key::F15 },
    { VK_F16, Key::F16 },
    { VK_F17, Key::F17 },
    { VK_F18, Key::F18 },
    { VK_F19, Key::F19 },
    { VK_F20, Key::F20 },
    { VK_F21, Key::F21 },
    { VK_F22, Key::F22 },
    { VK_F23, Key::F23 },
    { VK_F24, Key::F24 },
    { VK_NUMPAD0, Key::Kp0 },
    { VK_NUMPAD1, Key::Kp1 },
    { VK_NUMPAD2, Key::Kp2 },
    { VK_NUMPAD3, Key::Kp3 },
    { VK_NUMPAD4, Key::Kp4 },
    { VK_NUMPAD5, Key::Kp5 },
    { VK_NUMPAD6, Key::Kp6 },
    { VK_NUMPAD7, Key::Kp7 },
    { VK_NUMPAD8, Key::Kp8 },
    { VK_NUMPAD9, Key::Kp9 },
    { VK_DECIMAL, Key::KpDecimal },
    { VK_DIVIDE, Key::KpDivide },
    { VK_MULTIPLY, Key::KpMultiply },
    { VK_SUBTRACT, Key::KpSubtract },
    { VK_ADD, Key::KpAdd },
    { VK_OEM_NEC_EQUAL, Key::KpEqual },
    { VK_LSHIFT, Key::LeftShift },
    { VK_LCONTROL, Key::LeftControl },
    { VK_LMENU, Key::LeftAlt },
    { VK_RSHIFT, Key::RightShift },
    { VK_RCONTROL, Key::RightControl },
    { VK_RMENU, Key::RightAlt },
    { VK_MENU, Key::Menu },
    };

    if (keyMappingTable.contains(vk))
        return keyMappingTable.at(vk);
    return Key::None;
}

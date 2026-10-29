#include "Stylus.h"

#include <algorithm>
#include <commctrl.h>
#include <stdexcept>

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Comctl32.lib")

Stylus::Stylus(HWND window) : window(window)
{
    if (!IsWindow(window))
        throw std::invalid_argument("Stylus requires a valid window handle");

    if (!SetWindowSubclass(
        window, WindowProc, 1, reinterpret_cast<DWORD_PTR>(this)))
    {
        this->window = nullptr;
        throw std::runtime_error("Failed to attach stylus input to the window");
    }
}

Stylus::~Stylus()
{
    if (window && IsWindow(window))
        RemoveWindowSubclass(window, WindowProc, 1);
}

LRESULT CALLBACK Stylus::WindowProc(
    HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
    UINT_PTR subclassId, DWORD_PTR refData)
{
    auto* self = reinterpret_cast<Stylus*>(refData);
    if (!self)
        return DefSubclassProc(hwnd, msg, wParam, lParam);

    if (msg == WM_POINTERDOWN || msg == WM_POINTERUPDATE ||
        msg == WM_POINTERUP)
    {
        UINT32 id = GET_POINTERID_WPARAM(wParam);
        POINTER_INPUT_TYPE type{};

        if (GetPointerType(id, &type) && type == PT_PEN)
        {
            POINTER_PEN_INFO info{};

            if (GetPointerPenInfo(id, &info))
            {
                const auto& pi = info.pointerInfo;
                POINT clientPoint = pi.ptPixelLocation;
                if (!ScreenToClient(hwnd, &clientPoint))
                    return DefSubclassProc(hwnd, msg, wParam, lParam);

                glm::vec2 newPoint(
                    static_cast<float>(clientPoint.x),
                    static_cast<float>(clientPoint.y));

                bool inContact =
                    (pi.pointerFlags & POINTER_FLAG_INCONTACT) != 0;
                bool inRange =
                    (pi.pointerFlags & POINTER_FLAG_INRANGE) != 0;

                float newPressure = 0.0f;
                if (info.penMask & PEN_MASK_PRESSURE)
                {
                    newPressure = std::clamp(
                        static_cast<float>(info.pressure) / 1024.0f,
                        0.0f, 1.0f);
                }

                std::lock_guard<std::mutex> lock(self->mutex);
                self->point = newPoint;
                self->pressure = newPressure;
                bool wasTouching = self->pendingTouching;
                self->pendingHovering = inRange;
                self->pendingTouching = inContact;

                if (inContact && !wasTouching)
                    self->pendingBegin = true;

                if (!inContact && wasTouching)
                    self->pendingEnd = true;
            }
        }
    }

    if (msg == WM_NCDESTROY)
    {
        RemoveWindowSubclass(hwnd, WindowProc, subclassId);
        self->window = nullptr;
    }

    return DefSubclassProc(hwnd, msg, wParam, lParam);
}

void Stylus::GetData()
{
    std::lock_guard<std::mutex> lock(mutex);

    beganTouching = pendingBegin;
    direction = beganTouching ? glm::vec2(0.0f) : point - lastPoint;
    lastPoint = point;
    lastPressure = pressure;

    endedTouching = pendingEnd;
    hovering = pendingHovering;
    touching = pendingTouching;

    pendingBegin = false;
    pendingEnd = false;

    if (hovering)
        notHoveringTimeout = 0;
    else
        ++notHoveringTimeout;
}

glm::vec2 Stylus::GetPoint() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return point;
}

glm::vec2 Stylus::GetDirection() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return direction;
}

float Stylus::GetPressure() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return pressure;
}

bool Stylus::IsHovering() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return hovering;
}

bool Stylus::IsTouching() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return touching;
}

bool Stylus::BeganTouching() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return beganTouching;
}

bool Stylus::EndedTouching() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return endedTouching;
}
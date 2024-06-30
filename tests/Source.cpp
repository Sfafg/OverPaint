#include <iostream>
#include <VG/vg.h>
#include <chrono>
#include <thread>
#include <sstream>
#include <glm/glm.hpp>
#include "Window.h"
#include "Spline.h"
#include "Input.h"
#include "StateMachine.h"
using namespace std::chrono_literals;


float mposx, mposy;
float pressure = 0;
bool backSpace = false;
bool shiftBackspace = false;
float r = 255, g = 255, b = 255, a = 255, size = 5;
enum class SMState
{
    None = 0,
    Idle,
    MainColor,
    Size,
};
enum class SMInputType
{
    None = 0,
    Key,
    Button,
    Pointer,
};
std::ostream& operator<<(std::ostream& os, const SMState& rhs)
{
    switch (rhs)
    {
    case SMState::Idle: return os << "Idle";
    case SMState::MainColor: return os << "MainColor";
    case SMState::Size: return os << "Size";
    default:return os;
    }
}
int main()
{
    Window window("OverPaint", { 1920,1080 });
    Spline spline(2000);

    StateMachine<SMState, SMInputType> stateMachine(SMState::Idle);

    stateMachine.transitions = {
        {
            SMState::Idle,
            [&spline](auto& machine, void* input, SMInputType type) {
                Key key = type == SMInputType::Key ? *(Key*) input : Key::None;

                switch (key)
                {
                case Key::Delete:
                    while (spline.GetControlPointSize() != 0)
                        spline.RemoveControlPoint();
                    break;
                case Key::Z:
                {
                    if (!Input::GetMods().IsSet(Mods::Control))
                        break;

                    if (spline.GetControlPointSize() > 0 &&
                        spline.GetRangeC(spline.GetControlPointSize() - 1, spline.GetControlPointSize())->IsReset()) spline.RemoveControlPoint();

                    while (spline.GetControlPointSize() != 0 && !spline.GetRangeC(spline.GetControlPointSize() - 1, spline.GetControlPointSize())->IsReset())
                            spline.RemoveControlPoint();
                    }
                    break;

                case Key::C:
                    std::cout << "Setting main color: ";
                    machine.PushState(SMState::MainColor);
                    break;

                case Key::S:
                    std::cout << "Setting size: ";
                    machine.PushState(SMState::Size);
                    break;

                default: break;
                }

                return SMState::None;
            }
        },
        {
            SMState::MainColor,
            [](auto& machine, void* input, SMInputType type) {
                Key key = type == SMInputType::Key ? *(Key*) input : Key::None;
                Button button = type == SMInputType::Button ? *(Button*) input : Button::None;

                if (button == Button::Left)
                {
                    bool isR = false, isG = false, isB = false, isA = false;
                    for (int i = 0; i < machine.input.size(); i++)
                    {
                        switch (machine.input[i])
                        {
                        case 'R':isR = true; break;
                        case 'G':isG = true; break;
                        case 'B':isB = true; break;
                        case 'A':isA = true; break;
                        default:break;
                        }

                        std::stringstream numberSS;
                        while ('0' <= machine.input[i] && machine.input[i] <= '9' && i < machine.input.size())
                        {
                            numberSS << machine.input[i];
                            i++;
                        }
                        if (numberSS.tellp() != std::streampos(0))
                        {
                            float value;
                            numberSS >> value;
                            r = isR ? value : r;
                            g = isG ? value : g;
                            b = isB ? value : b;
                            a = isA ? value : a;
                            isR = isG = isB = isA = false;
                            i--;
                        }
                    }
                    machine.input = "";
                    std::cout << '\n';
                    machine.PopState();
                    return SMState::Idle;
                }

                if (Key::Zero <= key && key <= Key::Nine && machine.input.size() != 0)
                {
                    machine.input += (char) key;
                    std::cout << (char) key;
                }

                switch (key)
                {
                case Key::Enter:
                {
                    bool isR = false, isG = false, isB = false, isA = false;
                    for (int i = 0; i < machine.input.size(); i++)
                    {
                        switch (machine.input[i])
                        {
                        case 'R':isR = true; break;
                        case 'G':isG = true; break;
                        case 'B':isB = true; break;
                        case 'A':isA = true; break;
                        default:break;
                        }

                        std::stringstream numberSS;
                        while ('0' <= machine.input[i] && machine.input[i] <= '9' && i < machine.input.size())
                        {
                            numberSS << machine.input[i];
                            i++;
                        }
                        if (numberSS.tellp() != std::streampos(0))
                        {
                            float value;
                            numberSS >> value;
                            r = isR ? value : r;
                            g = isG ? value : g;
                            b = isB ? value : b;
                            a = isA ? value : a;
                            isR = isG = isB = isA = false;
                            i--;
                        }
                    }
                    machine.input = "";
                    std::cout << '\n';
                    machine.PopState();
                    break;
                }
                case Key::R: machine.input += (char) key; std::cout << "R"; break;
                case Key::G: machine.input += (char) key; std::cout << "G"; break;
                case Key::B: machine.input += (char) key; std::cout << "B"; break;
                case Key::A: machine.input += (char) key; std::cout << "A"; break;
                case Key::Backspace:
                    if (machine.input.size() != 0)
                    {
                        machine.input.pop_back();
                        std::cout << "\b \b";
                    }
                default: return SMState::None;
                }
                return SMState::None;
            }
        },
        {
            SMState::Size,
            [](auto& machine, void* input, SMInputType type) {
                Key key = type == SMInputType::Key ? *(Key*) input : Key::None;
                Button button = type == SMInputType::Button ? *(Button*) input : Button::None;

                if (Key::Zero <= key && key <= Key::Nine)
                {
                    machine.input += (char) key;
                    std::cout << (char) key;
                }
                else if (key == Key::Backspace && machine.input.size() != 0)
                {
                    machine.input.pop_back();
                    std::cout << "\b \b";
                }
                else if (key == Key::Enter || button == Button::Left)
                {
                    std::cout << "\n";
                    std::stringstream ss;
                    ss << machine.input;
                    ss >> size;
                    machine.input = "";
                    if (button != Button::Left)machine.PopState();
                    else
                    {
                        std::cout << '\n';
                        return SMState::Idle;
                    }
                }
                return SMState::None;
            }
        },
    };

    Input();
    Input::SetKeyCallback([&spline, &window, &stateMachine](Key key, State state, Input::Device d) {
        if (key == Key::Escape && state == State::Press)
            window.Close();

        if (state == State::Release)stateMachine.SetInput(&key, SMInputType::Key);
        if (key == Key::Backspace && state == State::Press)
        {
            if (spline.GetControlPointSize() != 0)spline.RemoveControlPoint();
        }
        });

    Input::SetButtonCallback([&stateMachine](Button button, State state, Input::Device device) {
        if (state == State::Press)
            stateMachine.SetInput(&button, SMInputType::Button);

        });

    Input::SetPointerCallback([&spline](bool pressed, int x, int y, float pressure, Input::Device device) {
        if (device != Input::Device::Pen)
            return;

        if (pressure != 0)
            spline.AddControlPoint(ControlPoint({ x,y }, pressure * size, { r,g,b,a }));
        else
            spline.AddControlPoint(ControlPoint::Reset);
        });


    while (!window.ShouldClose())
    {
        window.AwaitAll();
        BufferDraw d = spline;
        if (d.indexCount != -1)window.Draw(d);
        else
        {
            std::this_thread::sleep_for(0.03s);
        }
        window.Present();
    }
    window.AwaitAll();

    return 0;
}
#include <iostream>
#include <VG/vg.h>
#include <chrono>
#include <thread>
#include <sstream>
#include <glm/glm.hpp>
#include "Window.h"
#include "Spline.h"
using namespace std::chrono_literals;


int main()
{
    Window window("OverPaint", { 1920,1080 });
    Spline spline(2000);

    spline.AddControlPoint({ {100,100},20,{255,255,255,255} });
    spline.AddControlPoint({ {300,300},40,{255,255,255,255} });
    spline.AddControlPoint({ {1400,100},20,{255,255,255,255} });
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
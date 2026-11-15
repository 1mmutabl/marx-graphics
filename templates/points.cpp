/*
 * Template 06 — Points
 *
 * File:        rectangle.cpp
 * Next:        circle.cpp
 * Author:      1mmutabl
 * Created:     2026-11-14
 * Updated:     2026-11-14
 *
 * Description:
 *   Shows how to draw other shapes with points.
 */

#include <vector>

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
    graphics::window Window;
    Window.enable_vsync(true);

    std::vector<graphics::point> Points = {
        { { 400, 50 }, { 255, 0, 0, 255 } },
        { { 425, 145 }, { 255, 100, 0, 255 } },
        { { 520, 95 }, { 255, 200, 0, 255 } },
        { { 475, 185 }, { 255, 255, 0, 255 } },
        { { 600, 220 }, { 0, 255, 0, 255 } },
        { { 485, 260 }, { 0, 255, 100, 255 } },
        { { 550, 390 }, { 0, 255, 255, 255 } },
        { { 435, 310 }, { 0, 100, 255, 255 } },
        { { 400, 450 }, { 0, 0, 255, 255 } },
        { { 365, 310 }, { 100, 0, 255, 255 } },
        { { 250, 390 }, { 200, 0, 255, 255 } },
        { { 315, 260 }, { 255, 0, 255, 255 } },
        { { 200, 220 }, { 255, 0, 150, 255 } },
        { { 325, 185 }, { 255, 0, 100, 255 } }
    };

    while (Window.is_open())
    {
        while (auto Event = Window.poll())
        {
            if (Event->is<events::Close>())
                Window.close();
        }

        Window.clear();
        Window.draw(Points);
        Window.swap();
    }

    return 0;
}

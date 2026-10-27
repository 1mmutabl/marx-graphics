/*
 * Template 04 — Circles
 *
 * File:        circle.cpp
 * Next:        line.cpp
 * Author:      1mmutabl
 * Created:     2026-10-26
 * Updated:     2026-10-26
 *
 * Description:
 *   Shows how to draw a simple circle.
 */

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
  graphics::window Window;
  Window.enable_vsync(true);

  graphics::circle Circle;
  Circle.set_position(marx::fVector2(200.f, 200.f));
  Circle.set_radius(100.f);
  Circle.set_color(graphics::color(255, 0, 255, 255));
  Circle.set_segments(50.f);

  while (Window.is_open())
  {
    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();
    }

    Window.clear();
    Window.draw(Circle);
    Window.swap();
  }

  return 0;
}

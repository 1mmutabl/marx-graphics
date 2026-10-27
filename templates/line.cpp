/*
 * Template 05 — Lines
 *
 * File:        line.cpp
 * Next:        text.cpp
 * Author:      1mmutabl
 * Created:     2026-10-26
 * Updated:     2026-10-26
 *
 * Description:
 *   Shows how to draw a simple line.
 */

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
  graphics::window Window;
  Window.enable_vsync(true);

  graphics::line Line;
  Line.set_start_position(marx::fVector2(100.f, 100.f));
  Line.set_end_position(marx::fVector2(150.f, 400.f));
  // Line.set_position({ marx::fVector2(100.f, 100.f), marx::fVector2(150.f,
  // 400.f) });

  Line.set_thickness(5.f);
  Line.set_color(graphics::color(0, 255, 255, 255));

  while (Window.is_open())
  {
    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();
    }

    Window.clear();
    Window.draw(Line);
    Window.swap();
  }

  return 0;
}

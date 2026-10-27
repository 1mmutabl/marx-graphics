/*
 * Template 03 — Rectangles
 *
 * File:        rectangle.cpp
 * Next:        circle.cpp
 * Author:      1mmutabl
 * Created:     2026-10-26
 * Updated:     2026-10-26
 *
 * Description:
 *   Shows how to draw a simple rectangle.
 */

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
  graphics::window Window;
  Window.enable_vsync(true);

  graphics::rectangle Rectangle;
  Rectangle.set_position(marx::fVector2(100.f, 100.f));
  Rectangle.set_size(marx::fVector2(200.f, 100.f));
  Rectangle.set_rotation(45.f);
  Rectangle.set_color(graphics::color(255, 255, 0, 0.3 * 255));

  while (Window.is_open())
  {
    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();
    }

    Window.clear();
    // Window.clear(graphics::color(0, 0, 0));
    Window.draw(Rectangle);
    Window.swap();
  }

  return 0;
}

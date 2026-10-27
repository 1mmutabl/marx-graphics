/*
 * Template 01 — Window
 *
 * File:        window.cpp
 * Next:        events.cpp
 * Author:      1mmutabl
 * Created:     2026-10-26
 * Updated:     2026-10-26
 *
 * Description:
 *   Creates a window and handles basic window events.
 */

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
  graphics::window Window;
  // graphics::window window{marx::fVector2(800, 600), "marx graphics"};

  Window.enable_vsync(true);
  // Window.set_fps_cap(60);

  while (Window.is_open())
  {
    while (const auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();
    }
  }

  return 0;
}

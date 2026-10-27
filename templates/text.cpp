/*
 * Template 06 — Text
 *
 * File:        text.cpp
 * Next:        clock.cpp
 * Author:      1mmutabl
 * Created:     2026-10-26
 * Updated:     2026-10-26
 *
 * Description:
 *   Shows how to render text.
 */

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
  graphics::window Window;
  Window.enable_vsync(true);

  graphics::font Font;
  Font.load("resources/childer/Childer.otf", 68);

  graphics::text Text(Font);
  Text.set_string("Hello, marx!");
  Text.set_position(marx::fVector2(100.f, 100.f));
  Text.set_font_size(Font.get_size());
  Text.set_color(graphics::color(255, 255, 255, 255));

  while (Window.is_open())
  {
    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();
    }

    Window.clear();
    Window.draw(Text);
    Window.swap();
  }

  return 0;
}

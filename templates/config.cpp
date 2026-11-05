/*
 * Template 09 — Window config
 *
 * File:        config.cpp
 * Author:      1mmutabl
 * Created:     2026-11-02
 * Updated:     2026-11-02
 *
 * Description:
 *   Shows how to configure a window before it is
 *   created, using config flags.
 */

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;
namespace keyboard = graphics::keyboard;

int main()
{
  graphics::window_config Config;
  Config.add(graphics::config::Centered);
  Config.add(graphics::config::Msaa);
  Config.add(graphics::config::Vsync);
  // Config.add(graphics::config::Maximized);
  // Config.add(graphics::config::Minimized);
  // Config.add(graphics::config::Fullscreen);
  // Config.add(graphics::config::Hidden);
  // Config.add(graphics::config::Undecorated);
  // Config.add(graphics::config::Transparent);
  // Config.add(graphics::config::Floating);
  // Config.add(graphics::config::Fixed);

  graphics::window Window(marx::iVector2(800, 600), "marx graphics", Config);

  graphics::clock Clock;

  graphics::rectangle Rectangle(marx::fVector2(300.f, 200.f),
                                marx::fVector2(200.f, 200.f));

  while (Window.is_open())
  {
    Clock.update();

    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();

      else if (auto Key = Event->get_if<events::KeyDown>())
      {
        if (Key->Key == keyboard::keys::F11 && !Key->Repeat)
          Window.set_fullscreen(!Window.is_fullscreen());
      }
    }

    Rectangle.set_rotation(Clock.get_time_passed() * 45.f);

    Window.clear(graphics::color(24, 24, 30, 255));
    Window.draw(Rectangle);
    Window.swap();
  }

  return 0;
}

/*
 * Template 07 — Clock
 *
 * File:        clock.cpp
 * Next:        keyboard.cpp
 * Author:      1mmutabl
 * Created:     2026-10-26
 * Updated:     2026-10-26
 *
 * Description:
 *   Shows how to use the clock class.
 */

#include <format>

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
  graphics::window Window;
  // Window.enable_vsync(true);

  graphics::font Font;
  Font.load("resources/childer/Childer.otf", 32);

  float LineHeight = Font.get_line_height() * 1.1f;

  graphics::text fpsLabel(Font, "FPS : 0", { 10.f, 10.f }, Font.get_size());
  graphics::text DeltaTimeLabel(
      Font, "Delta Time : 0", { 10.f, fpsLabel.get_position().y + LineHeight },
      Font.get_size());
  graphics::text TimeLabel(
      Font, "Time Passed : 0",
      { 10.f, DeltaTimeLabel.get_position().y + LineHeight });

  graphics::clock Clock;

  while (Window.is_open())
  {
    Clock.update();
    float DeltaTime = Clock.get_delta_time();

    int fps = Clock.get_average_fps();
    // int fps = Clock.get_fps();

    float TimePassed = Clock.get_time_passed();

    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();
    }

    fpsLabel.set_string(std::format("FPS : {}", fps));
    DeltaTimeLabel.set_string(std::format("Delta Time : {}", DeltaTime));
    TimeLabel.set_string(std::format("Time Passed : {}", TimePassed));

    Window.clear();
    Window.draw(fpsLabel);
    Window.draw(DeltaTimeLabel);
    Window.draw(TimeLabel);
    Window.swap();
  }

  return 0;
}

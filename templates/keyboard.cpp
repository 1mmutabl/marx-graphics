/*
 * Template 08 — Keyboard input
 *
 * File:        keyboard.cpp
 * Author:      1mmutabl
 * Created:     2026-10-26
 * Updated:     2026-10-26
 *
 * Description:
 *   Shows how to use keyboard input to create
 *   player movement.
 */

#include "marx/graphics-internal/input/keyboard.hpp"

#include <cmath>

#include "marx/graphics-internal/shapes/rectangle.hpp"
#include "marx/graphics-internal/vector.hpp"
#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;
namespace keyboard = graphics::keyboard;

namespace
{
constexpr const int Speed = 500;
}

int main()
{
  graphics::window Window;
  Window.enable_vsync(true);

  graphics::clock Clock;

  graphics::rectangle Player(marx::fVector2(100.f, 100.f));
  while (Window.is_open())
  {
    Clock.update();
    float DeltaTime = Clock.get_delta_time();

    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();
    }

    marx::fVector2 Velocity;

    if (keyboard::key_down(keyboard::keys::W) ||
        keyboard::key_down(keyboard::keys::Up))
      Velocity.y -= 1;
    if (keyboard::key_down(keyboard::keys::S) ||
        keyboard::key_down(keyboard::keys::Down))
      Velocity.y += 1;

    if (keyboard::key_down(keyboard::keys::A) ||
        keyboard::key_down(keyboard::keys::Left))
      Velocity.x -= 1;
    if (keyboard::key_down(keyboard::keys::D) ||
        keyboard::key_down(keyboard::keys::Right))
      Velocity.x += 1;

    const float Length =
        std::hypot(Velocity.x, Velocity.y); // Normalize velocity
    if (Length > 0)
    {
      Velocity.x /= Length;
      Velocity.y /= Length;
    }

    marx::fVector2 PlayerPosition = Player.get_position();
    Player.set_position(PlayerPosition + Velocity * Speed * DeltaTime);

    Window.clear();
    Window.draw(Player);
    Window.swap();
  }

  return 0;
}

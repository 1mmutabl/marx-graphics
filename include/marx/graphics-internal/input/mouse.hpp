#pragma once

namespace marx::graphics::mouse
{
enum class buttons
{
  Left,
  Middle,
  Right,

  Null
};

bool button_down(buttons Button);

} // namespace marx::graphics::mouse

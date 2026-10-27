#pragma once

#include <iostream>
#include <utility>
#include <variant>

#include "input/keyboard.hpp"

namespace marx::graphics::events
{
struct Close
{
};

struct Moved
{
  int x, y;
};
struct Resized
{
  int x, y;
};

struct Focused
{
};
struct Unfocused
{
};

struct KeyDown
{
  keyboard::keys Key;
  char           Ascii;
};

struct KeyUp
{
  keyboard::keys Key;
  char           Ascii;
};

struct MouseMove
{
  uint x, y;
};

struct ButtonDown
{
  uint Button;
};

struct ButtonUp
{
  uint Button;
};
} // namespace marx::graphics::events

namespace marx::graphics
{
using EventData =
    std::variant<std::monostate, events::Close, events::Moved, events::Resized,
                 events::Focused, events::Unfocused, events::KeyDown,
                 events::KeyUp, events::MouseMove, events::ButtonDown,
                 events::ButtonUp>;

class event
{
public:

  template<typename T, typename... Args> void set(Args &&...args)
  {
    if (!std::holds_alternative<std::monostate>(Event_))
    {
      std::cerr << "Error: Event can only be set once.\n";
      return;
    }

    Event_.template emplace<T>(T{ std::forward<Args>(args)... });
  }

  template<typename T> bool is() const
  {
    return std::holds_alternative<T>(Event_);
  }

  template<typename T> T &get()
  {
    return std::get<T>(Event_);
  }

  template<typename T> const T &get() const
  {
    return std::get<T>(Event_);
  }

  template<typename T> T *get_if()
  {
    return std::get_if<T>(&Event_);
  }

  template<typename T> const T *get_if() const
  {
    return std::get_if<T>(&Event_);
  }

private:

  EventData Event_;
};
} // namespace marx::graphics

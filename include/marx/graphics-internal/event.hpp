#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <variant>

#include "input/keyboard.hpp"
#include "input/mouse.hpp"

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
  keyboard::keys      Key;
  char                Ascii;
  keyboard::modifiers Modifiers;
  bool                Repeat;
};

struct KeyUp
{
  keyboard::keys      Key;
  char                Ascii;
  keyboard::modifiers Modifiers;
};

struct TextInput
{
  std::string Text;
};

struct MouseMove
{
  int x, y;
};

struct MouseEntered
{
  int x, y;
};

struct MouseLeft
{
  int x, y;
};

struct ButtonDown
{
  mouse::buttons      Button;
  int                 x, y;
  keyboard::modifiers Modifiers;
};

struct ButtonUp
{
  mouse::buttons      Button;
  int                 x, y;
  keyboard::modifiers Modifiers;
};

struct Scrolled
{
  int                 x, y;
  float               DeltaX, DeltaY;
  keyboard::modifiers Modifiers;
};
} // namespace marx::graphics::events

namespace marx::graphics
{
using EventData =
    std::variant<std::monostate, events::Close, events::Moved, events::Resized,
                 events::Focused, events::Unfocused, events::KeyDown,
                 events::KeyUp, events::TextInput, events::MouseMove,
                 events::MouseEntered, events::MouseLeft, events::ButtonDown,
                 events::ButtonUp, events::Scrolled>;

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

/*
 * Template 02 — Events
 *
 * File:        events.cpp
 * Next:        rectangle.cpp
 * Author:      1mmutabl
 * Created:     2026-10-26
 * Updated:     2026-10-30
 *
 * Description:
 *   Shows all available events.
 */

#include <iostream>

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
  graphics::window Window;

  while (Window.is_open())
  {
    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();

      else if (Event->is<events::Focused>())
        std::cout << "Focused\n" << std::endl;
      else if (Event->is<events::Unfocused>())
        std::cout << "Unfocused\n" << std::endl;

      else if (auto Moved = Event->get_if<events::Moved>())
        std::cout << "Moved:\n"
                     "x: "
                  << Moved->x
                  << "\n"
                     "y: "
                  << Moved->y << "\n"
                  << std::endl;
      else if (auto Resized = Event->get_if<events::Resized>())
        std::cout << "Resized:\n"
                     "x: "
                  << Resized->x
                  << "\n"
                     "y: "
                  << Resized->y << "\n"
                  << std::endl;

      else if (auto Key = Event->get_if<events::KeyDown>())
        std::cout << "Key down:\n"
                     "Key:    "
                  << static_cast<int>(Key->Key)
                  << "\n"
                     "Ascii:  "
                  << Key->Ascii
                  << "\n"
                     "Repeat: "
                  << Key->Repeat
                  << "\n"
                     "Shift:  "
                  << Key->Modifiers.Shift
                  << "\n"
                     "Ctrl:   "
                  << Key->Modifiers.Control
                  << "\n"
                     "Alt:    "
                  << Key->Modifiers.Alt
                  << "\n"
                     "Super:  "
                  << Key->Modifiers.Super << "\n"
                  << std::endl;
      else if (auto Key = Event->get_if<events::KeyUp>())
        std::cout << "Key up:\n"
                     "Key:   "
                  << static_cast<int>(Key->Key)
                  << "\n"
                     "Ascii: "
                  << Key->Ascii << "\n"
                  << std::endl;
      else if (auto Text = Event->get_if<events::TextInput>())
        std::cout << "Text input: " << Text->Text << "\n" << std::endl;

      else if (auto Mouse = Event->get_if<events::MouseMove>())
        std::cout << "Mouse move:\n"
                     "x: "
                  << Mouse->x
                  << "\n"
                     "y: "
                  << Mouse->y << "\n"
                  << std::endl;
      else if (auto Mouse = Event->get_if<events::MouseEntered>())
        std::cout << "Mouse entered:\n"
                     "x: "
                  << Mouse->x
                  << "\n"
                     "y: "
                  << Mouse->y << "\n"
                  << std::endl;
      else if (auto Mouse = Event->get_if<events::MouseLeft>())
        std::cout << "Mouse left:\n"
                     "x: "
                  << Mouse->x
                  << "\n"
                     "y: "
                  << Mouse->y << "\n"
                  << std::endl;
      else if (auto Mouse = Event->get_if<events::ButtonDown>())
        std::cout << "Button down:\n"
                     "Button: "
                  << static_cast<int>(Mouse->Button)
                  << "\n"
                     "x:      "
                  << Mouse->x
                  << "\n"
                     "y:      "
                  << Mouse->y << "\n"
                  << std::endl;
      else if (auto Mouse = Event->get_if<events::ButtonUp>())
        std::cout << "Button up:\n"
                     "Button: "
                  << static_cast<int>(Mouse->Button)
                  << "\n"
                     "x:      "
                  << Mouse->x
                  << "\n"
                     "y:      "
                  << Mouse->y << "\n"
                  << std::endl;
      else if (auto Scroll = Event->get_if<events::Scrolled>())
        std::cout << "Scrolled:\n"
                     "x:       "
                  << Scroll->x
                  << "\n"
                     "y:       "
                  << Scroll->y
                  << "\n"
                     "Delta x: "
                  << Scroll->DeltaX
                  << "\n"
                     "Delta y: "
                  << Scroll->DeltaY << "\n"
                  << std::endl;
    }
  }

  return 0;
}

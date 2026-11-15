/*
 * Template 12 — Icons
 *
 * File:        config.cpp
 * Author:      1mmutabl
 * Created:     2026-11-15
 * Updated:     2026-11-15
 *
 * Description:
 *   Shows how to change the icon of the window
 */

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
    graphics::window Window(marx::iVector2(800, 600), "marx graphics");

    graphics::texture Icon("resources/logo.png");
    Window.set_icon(Icon);

    graphics::font Font("resources/Roboto.ttf");

    graphics::text Text(Font);
    Text.set_string("Look at the icon of the window!");
    Text.set_position({ 100.f, 100.f });

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

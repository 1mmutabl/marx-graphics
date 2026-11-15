/*
 * Template 11 — Textures
 *
 * File:        config.cpp
 * Author:      1mmutabl
 * Created:     2026-11-14
 * Updated:     2026-11-14
 *
 * Description:
 *   Shows how to render textures
 */

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

int main()
{
    graphics::window Window(marx::iVector2(800, 600), "marx graphics");

    marx::iVector2 WindowSize = Window.get_size();

    graphics::texture Texture("resources/hill.jpg");
    graphics::sprite  Sprite(Texture);

    Sprite.set_size(
        { static_cast<float>(WindowSize.x), static_cast<float>(WindowSize.y) });

    while (Window.is_open())
    {
        while (auto Event = Window.poll())
        {
            if (Event->is<events::Close>())
                Window.close();

            else if (const auto Resize = Event->get_if<events::Resized>())
            {
                WindowSize.x = Resize->x;
                WindowSize.y = Resize->y;

                Sprite.set_size({ static_cast<float>(WindowSize.x),
                                  static_cast<float>(WindowSize.y) });
            }
        }

        Window.clear();
        Window.draw(Sprite);
        Window.swap();
    }

    return 0;
}

/*
 * Template 00 — Cube
 *
 * File:        config.cpp
 * Author:      1mmutabl
 * Created:     2026-11-14
 * Updated:     2026-11-14
 *
 * Description:
 *   Shows how to render a cube using points.
 */

#include <cmath>
#include <numbers>
#include <vector>

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;

struct fVector3
{
    float x, y, z;

    fVector3(float x = 0.f, float y = 0.f, float z = 0.f)
        : x(x)
        , y(y)
        , z(z)
    {
    }
};

struct cube
{
    fVector3 Position;
    fVector3 Rotation;

    float           Size;
    graphics::color Color;

    void draw(const graphics::window &Window) const
    {
        float Rx = Rotation.x * std::numbers::pi / 180.f;
        float Ry = Rotation.y * std::numbers::pi / 180.f;
        float Rz = Rotation.z * std::numbers::pi / 180.f;

        float CX = std::cos(Rx);
        float SX = std::sin(Rx);

        float CY = std::cos(Ry);
        float SY = std::sin(Ry);

        float CZ = std::cos(Rz);
        float SZ = std::sin(Rz);

        float Half = Size / 2.f;

        std::array<fVector3, 8> Vertices = { { { -Half, -Half, -Half },
                                               { Half, -Half, -Half },
                                               { Half, Half, -Half },
                                               { -Half, Half, -Half },

                                               { -Half, -Half, Half },
                                               { Half, -Half, Half },
                                               { Half, Half, Half },
                                               { -Half, Half, Half } } };

        std::array<marx::fVector2, 8> Projected;

        for (std::size_t i = 0; i < Vertices.size(); ++i)
        {
            float x = Vertices[i].x;
            float y = Vertices[i].y;
            float z = Vertices[i].z;

            float Y = y * CX - z * SX;
            float Z = y * SX + z * CX;

            y = Y;
            z = Z;

            float X = x * CY + z * SY;
            Z       = -x * SY + z * CY;

            x = X;
            z = Z;

            X = x * CZ - y * SZ;
            Y = x * SZ + y * CZ;

            x = X;
            y = Y;

            x += Position.x;
            y += Position.y;
            z += Position.z;

            float Perspective = 500.f / z;

            Projected[i].x = 400.f + x * Perspective;
            Projected[i].y = 300.f + y * Perspective;
        }

        constexpr std::array<std::array<int, 4>, 6> Faces = {
            { { { 0, 1, 2, 3 } },
              { { 4, 7, 6, 5 } },
              { { 0, 4, 5, 1 } },
              { { 3, 2, 6, 7 } },
              { { 0, 3, 7, 4 } },
              { { 1, 5, 6, 2 } } }
        };

        std::array<graphics::color, 6> Colors = { { { 255, 80, 80, 255 },
                                                    { 255, 180, 80, 255 },
                                                    { 80, 255, 80, 255 },
                                                    { 80, 80, 255, 255 },
                                                    { 255, 255, 80, 255 },
                                                    { 180, 80, 255, 255 } } };

        for (std::size_t i = 0; i < Faces.size(); ++i)
        {
            std::vector<graphics::point> Points;

            for (int Index : Faces[i])
            {
                Points.push_back({ Projected[Index], Colors[i] });
            }

            Window.draw(Points);
        }
    }

    cube(fVector3 Position = { 0.f, 0.f, 500.f },
         fVector3 Rotation = { 25.f, 25.f, 0.f }, float Size = 150.f,
         graphics::color Color = { 255, 255, 255, 255 })
        : Position(Position)
        , Rotation(Rotation)
        , Size(Size)
        , Color(Color)
    {
    }
};

int main()
{
    graphics::window Window(marx::iVector2(800, 600), "marx graphics");

    cube Cube;
    Cube.Rotation = fVector3(0, 0, 0);

    while (Window.is_open())
    {
        while (auto Event = Window.poll())
        {
            if (Event->is<events::Close>())
                Window.close();
        }

        Window.clear();
        Cube.draw(Window);
        Window.swap();
    }

    return 0;
}

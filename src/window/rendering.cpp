#include "marx/graphics-internal/window.hpp"

#include <glad/gl.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <numbers>
#include <thread>
#include <vector>

#include "internal.hpp"

using namespace marx::graphics::internal;

namespace marx::graphics
{
void window::clear(color Color) const
{
    const float Alpha = Transparent_ ? Color.a / 255.f : 1.f;

    glDisable(GL_SCISSOR_TEST);
    glClearColor(Color.r / 255.f * Alpha, Color.g / 255.f * Alpha,
                 Color.b / 255.f * Alpha, Alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::set_clip(fVector2 Position, fVector2 Size) const
{
    const iVector2 WindowSize = get_size();

    const int x      = static_cast<int>(std::round(Position.x));
    const int Width  = std::max(0, static_cast<int>(std::round(Size.x)));
    const int Height = std::max(0, static_cast<int>(std::round(Size.y)));
    const int y =
        WindowSize.y - static_cast<int>(std::round(Position.y)) - Height;

    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, Width, Height);
}

void window::reset_clip() const
{
    glDisable(GL_SCISSOR_TEST);
}

void window::draw(const rectangle &Rectangle) const
{
    fVector2 Position     = Rectangle.get_position();
    fVector2 Size         = Rectangle.get_size();
    color    Color        = Rectangle.get_color();
    float    Rotation     = Rectangle.get_rotation();
    float    CornerRadius = std::max(
        0.f, std::min(Rectangle.get_corner_radius(),
                      std::min(Size.x, Size.y) / 2.f));

    float x1 = Position.x;
    float y1 = Position.y;
    float x2 = Position.x + Size.x;
    float y2 = Position.y + Size.y;

    float r = Color.r / 255.f;
    float g = Color.g / 255.f;
    float b = Color.b / 255.f;
    float a = Color.a / 255.f;

    std::array<float, 36> Data = { x1, y1, r, g, b, a, x2, y1, r, g, b, a,
                                   x2, y2, r, g, b, a, x1, y1, r, g, b, a,
                                   x2, y2, r, g, b, a, x1, y2, r, g, b, a };

    glUseProgram(Shader_);

    GLint ScreenSizeLocation = glGetUniformLocation(Shader_, "ScreenSize");
    glUniform2f(ScreenSizeLocation, static_cast<float>(Size_.x),
                static_cast<float>(Size_.y));

    GLint RotationLocation = glGetUniformLocation(Shader_, "Rotation");
    glUniform1f(RotationLocation, Rotation);

    GLint SizeLocation = glGetUniformLocation(Shader_, "Size");
    glUniform2f(SizeLocation, Size.x, Size.y);

    float centerX = Position.x + Size.x / 2.0f;
    float centerY = Position.y + Size.y / 2.0f;

    GLint CenterLocation = glGetUniformLocation(Shader_, "Center");
    glUniform2f(CenterLocation, centerX, centerY);

    GLint RadiusLocation = glGetUniformLocation(Shader_, "CornerRadius");
    glUniform1f(RadiusLocation, CornerRadius);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Data), Data.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void window::draw(const line &Line) const
{
    float Thickness = Line.get_thickness();
    color Color     = Line.get_color();

    fVector2 Start = Line.get_start_position();
    fVector2 End   = Line.get_end_position();

    fVector2 Delta  = End - Start;
    float    Length = std::sqrt(std::pow(Delta.x, 2) + std::pow(Delta.y, 2));

    float Angle = std::atan2(Delta.x, Delta.y) * 180.f / std::numbers::pi;

    fVector2 MidPoint = (Start + End) / 2;

    rectangle Rectangle;
    Rectangle.set_size(fVector2(Thickness, Length));
    Rectangle.set_rotation(Angle);
    Rectangle.set_color(Color);
    Rectangle.set_corner_radius(0.f);

    Rectangle.set_position(fVector2(MidPoint.x - Rectangle.get_size().x / 2.f,
                                    MidPoint.y - Rectangle.get_size().y / 2.f));

    draw(Rectangle);
}

void window::draw(const circle &Circle) const
{
    const fVector2 Position = Circle.get_position();
    const float    Radius   = Circle.get_radius();
    const color    Color    = Circle.get_color();
    const int      Segments = std::max(3, Circle.get_segments());

    if (Radius <= 0.f)
        return;

    const fVector2 Center(Position.x + Radius, Position.y + Radius);

    const float Step =
        2.f * std::numbers::pi_v<float> / static_cast<float>(Segments);

    const float r = Color.r / 255.f;
    const float g = Color.g / 255.f;
    const float b = Color.b / 255.f;
    const float a = Color.a / 255.f;

    std::vector<float> Data;
    Data.reserve(static_cast<std::size_t>(Segments) * 18);

    for (int i = 0; i < Segments; ++i)
    {
        const float A0 = static_cast<float>(i) * Step;
        const float A1 = static_cast<float>(i + 1) * Step;

        const fVector2 P0(Center.x + Radius * std::cos(A0),
                          Center.y + Radius * std::sin(A0));

        const fVector2 P1(Center.x + Radius * std::cos(A1),
                          Center.y + Radius * std::sin(A1));

        Data.insert(Data.end(), { Center.x, Center.y, r, g, b, a, P0.x, P0.y, r,
                                  g, b, a, P1.x, P1.y, r, g, b, a });
    }

    glUseProgram(Shader_);

    GLint ScreenSizeLocation = glGetUniformLocation(Shader_, "ScreenSize");
    glUniform2f(ScreenSizeLocation, static_cast<float>(Size_.x),
                static_cast<float>(Size_.y));

    GLint RotationLocation = glGetUniformLocation(Shader_, "Rotation");
    glUniform1f(RotationLocation, 0.f);

    GLint CenterLocation = glGetUniformLocation(Shader_, "Center");
    glUniform2f(CenterLocation, Center.x, Center.y);

    GLint SizeLocation = glGetUniformLocation(Shader_, "Size");
    glUniform2f(SizeLocation, Radius * 2, Radius * 2);

    GLint CornerRadiusLocation = glGetUniformLocation(Shader_, "CornerRadius");
    glUniform1f(CornerRadiusLocation, 0.f);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(Data.size() * sizeof(float)),
                 Data.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, Segments * 3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void window::draw(const text &Text) const
{
    const std::vector<float> &Vertices = Text.get_vertices();

    if (Vertices.empty())
        return;

    glUseProgram(TextShader_);

    glUniform2f(glGetUniformLocation(TextShader_, "ScreenSize"),
                static_cast<float>(Size_.x), static_cast<float>(Size_.y));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Text.get_font()->get_texture());
    glUniform1i(glGetUniformLocation(TextShader_, "Atlas"), 0);

    glBindVertexArray(TextVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, TextVBO_);

    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(Vertices.size() * sizeof(float)),
                 Vertices.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0,
                 static_cast<GLsizei>(Vertices.size() / text::FloatsPerVertex));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void window::draw(const std::vector<point> &Points) const
{
    if (Points.size() < 2)
        return;

    std::vector<float> Data;
    Data.reserve(Points.size() * 6);

    for (const point &Point : Points)
    {
        float x = Point.Position.x;
        float y = Point.Position.y;

        float r = Point.Color.r / 255.f;
        float g = Point.Color.g / 255.f;
        float b = Point.Color.b / 255.f;
        float a = Point.Color.a / 255.f;

        Data.insert(Data.end(), { x, y, r, g, b, a });
    }

    glUseProgram(Shader_);

    GLint ScreenSizeLocation = glGetUniformLocation(Shader_, "ScreenSize");
    glUniform2f(ScreenSizeLocation, static_cast<float>(Size_.x),
                static_cast<float>(Size_.y));

    GLint RotationLocation = glGetUniformLocation(Shader_, "Rotation");
    glUniform1f(RotationLocation, 0.0f);

    GLint CenterLocation = glGetUniformLocation(Shader_, "Center");
    glUniform2f(CenterLocation, 0.0f, 0.0f);

    GLint CornerRadiusLocation = glGetUniformLocation(Shader_, "CornerRadius");
    glUniform1f(CornerRadiusLocation, 0.0f);

    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    glBufferData(GL_ARRAY_BUFFER, Data.size() * sizeof(float), Data.data(),
                 GL_DYNAMIC_DRAW);

    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(Points.size()));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void window::draw(const sprite &Sprite) const
{
    marx::fVector2 Position = Sprite.get_position();
    marx::fVector2 Size     = Sprite.get_size();

    marx::iVector2 ScreenSize = get_size();

    float Vertices[] = { Position.x,          Position.y,          0.0f, 0.0f,
                         Position.x + Size.x, Position.y,          1.0f, 0.0f,
                         Position.x + Size.x, Position.y + Size.y, 1.0f, 1.0f,

                         Position.x,          Position.y,          0.0f, 0.0f,
                         Position.x + Size.x, Position.y + Size.y, 1.0f, 1.0f,
                         Position.x,          Position.y + Size.y, 0.0f, 1.0f };

    glUseProgram(SpriteShader_);

    glUniform2f(glGetUniformLocation(SpriteShader_, "ScreenSize"), ScreenSize.x,
                ScreenSize.y);

    glUniform2f(glGetUniformLocation(SpriteShader_, "Center"),
                Position.x + Size.x / 2.0f, Position.y + Size.y / 2.0f);

    glUniform1f(glGetUniformLocation(SpriteShader_, "Rotation"), 0.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Sprite.get_id());

    glUniform1i(glGetUniformLocation(SpriteShader_, "Texture"), 0);

    glBindVertexArray(SpriteVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, SpriteVBO_);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void window::swap()
{
    if (!Display_ || !Window_ || !Context_)
        return;

    glXSwapBuffers(Display_, Window_);

    if (fpsCap_ == 0)
    {
        FrameTimerStarted_ = false;
        return;
    }

    const auto FrameDuration = std::chrono::duration_cast<Clock::duration>(
        std::chrono::duration<double>(1.0 / static_cast<double>(fpsCap_)));

    const auto Now = Clock::now();

    if (!FrameTimerStarted_)
    {
        NextFrame_         = Now;
        FrameTimerStarted_ = true;
    }

    NextFrame_ += FrameDuration;

    if (NextFrame_ > Now)
        std::this_thread::sleep_until(NextFrame_);
    else
        NextFrame_ = Now;
}
} // namespace marx::graphics

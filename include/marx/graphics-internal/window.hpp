#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <sys/types.h>

#include "color.hpp"
#include "event.hpp"
#include "shapes/circle.hpp"
#include "shapes/line.hpp"
#include "shapes/rectangle.hpp"
#include "text/text.hpp"
#include "vector.hpp"

struct _XDisplay;

namespace marx::graphics
{
_XDisplay *get_display();

enum class cursors
{
  Arrow,
  Text,
  Hand,
  Crosshair,
  Wait,
  Move,
  ResizeHorizontal,
  ResizeVertical
};

class window
{
public:

  window(iVector2 Size = { 800, 600 }, std::string Name = "marx graphics");
  ~window();

  void     set_size(iVector2 Size);
  iVector2 get_size() const;

  void set_min_size(iVector2 Size);
  void set_max_size(iVector2 Size);
  void set_resizable(bool Resizable);

  void     set_position(iVector2 Position);
  iVector2 get_position() const;

  void        set_name(std::string Name);
  std::string get_name() const;

  void set_fullscreen(bool Fullscreen);
  bool is_fullscreen() const;

  bool is_focused() const;

  iVector2 get_mouse_position() const;

  void set_cursor(cursors Cursor);
  void set_cursor_visible(bool Visible);

  void enable_vsync(bool Enable);
  void set_fps_cap(uint fpsCap);

  std::unique_ptr<event> poll();

  void clear(color Color = { 0, 0, 0 }) const;

  void set_clip(fVector2 Position, fVector2 Size) const;
  void reset_clip() const;

  void draw(const rectangle &Rectangle) const;
  void draw(const line &Line) const;
  void draw(const circle &Circle) const;
  void draw(const text &Text) const;

  void swap();

  void close();
  bool is_open() const;

private:

  iVector2    Size_;
  iVector2    Position_;
  std::string Name_;

  int                                   fpsCap_ = 0;
  std::chrono::steady_clock::time_point NextFrame_;
  bool                                  FrameTimerStarted_ = 0;

  bool Closed_ = false;

  uint Shader_;
  uint VAO_;
  uint VBO_;

  uint TextShader_;
  uint TextVAO_;
  uint TextVBO_;
};
} // namespace marx::graphics

#pragma once

#include <glad/gl.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/XF86keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <array>
#include <chrono>
#include <deque>
#include <memory>
#include <string>
#include <vector>

#include "marx/graphics-internal/event.hpp"
#include "marx/graphics-internal/input/keyboard.hpp"
#include "marx/graphics-internal/input/mouse.hpp"
#include "marx/graphics-internal/text/utf8.hpp"
#include "marx/graphics-internal/vector.hpp"
#include "marx/graphics-internal/window.hpp"
#include "marx/graphics-internal/window_config.hpp"

using PFNGLXSWAPINTERVALEXTPROC = void (*)(Display *, GLXDrawable, int);
using string                    = std::string;
using Clock                     = std::chrono::steady_clock;
namespace keyboard              = marx::graphics::keyboard;
namespace mouse                 = marx::graphics::mouse;
namespace utf8                  = marx::graphics::utf8;

namespace marx::graphics::internal
{
extern Display *Display_;
extern int      Scr_;
extern Window   Root_;
extern Window   Window_;
extern Atom     WM_DELETE_;

extern GLXContext Context_;

extern XIM InputMethod_;
extern XIC InputContext_;

extern PFNGLXSWAPINTERVALEXTPROC SwapInterval_;

extern std::deque<std::unique_ptr<event>> Pending_;
extern std::array<bool, 256>              HeldKeys_;
extern bool                               Focused_;
extern bool                               Transparent_;

extern iVector2 MinSize_;
extern iVector2 MaxSize_;

extern iVector2 ReportedSize_;
extern iVector2 ReportedPosition_;

extern std::array<Cursor, 8> Cursors_;
extern Cursor                BlankCursor_;
extern cursors               CurrentCursor_;
extern bool                  CursorVisible_;
extern bool                  CursorApplied_;

extern const string VertShader;
extern const string FragShader;
extern const string TextVertShader;
extern const string TextFragShader;
extern const string SpriteVertShader;
extern const string SpriteFragShader;

keyboard::keys      x11_to_kb(KeySym Key);
keyboard::keys      get_key(XKeyEvent &Event);
string              get_text(XKeyEvent &Event);
bool                is_printable(const string &Text);
keyboard::modifiers get_modifiers(unsigned int State);
mouse::buttons      x11_to_mouse(unsigned int Button);

void         apply_cursor();
void         apply_size_hints();
XVisualInfo *choose_visual(bool Alpha, bool Multisample);
void apply_config(const window_config &Config, iVector2 Size, iVector2 Position);

uint make_shader(const string &Vertex, const string &Fragment);
} // namespace marx::graphics::internal

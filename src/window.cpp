#include "marx/graphics-internal/window.hpp"

#include <glad/gl.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/X.h>
#include <X11/XF86keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <array>
#include <chrono>
#include <cmath>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "marx/graphics-internal/color.hpp"
#include "marx/graphics-internal/event.hpp"
#include "marx/graphics-internal/input/keyboard.hpp"
#include "marx/graphics-internal/shapes/circle.hpp"
#include "marx/graphics-internal/shapes/line.hpp"
#include "marx/graphics-internal/shapes/rectangle.hpp"
#include "marx/graphics-internal/text/font.hpp"
#include "marx/graphics-internal/text/text.hpp"
#include "marx/graphics-internal/vector.hpp"

using PFNGLXSWAPINTERVALEXTPROC = void (*)(Display *, GLXDrawable, int);
using string                    = std::string;
using Clock                     = std::chrono::steady_clock;
namespace keyboard              = marx::graphics::keyboard;

namespace
{
static Display *Display_ = nullptr;
static int      Scr_;
static Window   Root_;
static Window   Window_;
static XEvent   Event_;
static Atom     WM_DELETE_;

static XVisualInfo *Visual_   = nullptr;
static Colormap     Colormap_ = 0;
static GLXContext   Context_  = nullptr;

PFNGLXSWAPINTERVALEXTPROC SwapInterval_;

std::vector<float> Data_;

keyboard::keys x11_to_kb(KeySym Key)
{
  using K = keyboard::keys;

  switch (Key)
  {
    case XK_a:
    case XK_A:
      return K::A;
    case XK_b:
    case XK_B:
      return K::B;
    case XK_c:
    case XK_C:
      return K::C;
    case XK_d:
    case XK_D:
      return K::D;
    case XK_e:
    case XK_E:
      return K::E;
    case XK_f:
    case XK_F:
      return K::F;
    case XK_g:
    case XK_G:
      return K::G;
    case XK_h:
    case XK_H:
      return K::H;
    case XK_i:
    case XK_I:
      return K::I;
    case XK_j:
    case XK_J:
      return K::J;
    case XK_k:
      return K::K;
    case XK_l:
      return K::L;
    case XK_m:
      return K::M;
    case XK_n:
      return K::N;
    case XK_o:
      return K::O;
    case XK_p:
      return K::P;
    case XK_q:
      return K::Q;
    case XK_r:
      return K::R;
    case XK_s:
      return K::S;
    case XK_t:
      return K::T;
    case XK_u:
      return K::U;
    case XK_v:
      return K::V;
    case XK_w:
      return K::W;
    case XK_x:
      return K::X;
    case XK_y:
      return K::Y;
    case XK_z:
      return K::Z;

    case XK_0:
      return K::Zero;
    case XK_1:
      return K::One;
    case XK_2:
      return K::Two;
    case XK_3:
      return K::Three;
    case XK_4:
      return K::Four;
    case XK_5:
      return K::Five;
    case XK_6:
      return K::Six;
    case XK_7:
      return K::Seven;
    case XK_8:
      return K::Eight;
    case XK_9:
      return K::Nine;

    case XK_F1:
      return K::F1;
    case XK_F2:
      return K::F2;
    case XK_F3:
      return K::F3;
    case XK_F4:
      return K::F4;
    case XK_F5:
      return K::F5;
    case XK_F6:
      return K::F6;
    case XK_F7:
      return K::F7;
    case XK_F8:
      return K::F8;
    case XK_F9:
      return K::F9;
    case XK_F10:
      return K::F10;
    case XK_F11:
      return K::F11;
    case XK_F12:
      return K::F12;
    case XK_F13:
      return K::F13;
    case XK_F14:
      return K::F14;
    case XK_F15:
      return K::F15;
    case XK_F16:
      return K::F16;
    case XK_F17:
      return K::F17;
    case XK_F18:
      return K::F18;
    case XK_F19:
      return K::F19;
    case XK_F20:
      return K::F20;
    case XK_F21:
      return K::F21;
    case XK_F22:
      return K::F22;
    case XK_F23:
      return K::F23;
    case XK_F24:
      return K::F24;

    case XK_Shift_L:
      return K::LeftShift;
    case XK_Shift_R:
      return K::RightShift;
    case XK_Control_L:
      return K::LeftControl;
    case XK_Control_R:
      return K::RightControl;
    case XK_Alt_L:
      return K::LeftAlt;
    case XK_Alt_R:
      return K::RightAlt;
    case XK_Super_L:
      return K::LeftSuper;
    case XK_Super_R:
      return K::RightSuper;
    case XK_Caps_Lock:
      return K::CapsLock;
    case XK_Num_Lock:
      return K::NumLock;
    case XK_Scroll_Lock:
      return K::ScrollLock;

    case XK_Up:
      return K::Up;
    case XK_Down:
      return K::Down;
    case XK_Left:
      return K::Left;
    case XK_Right:
      return K::Right;
    case XK_Home:
      return K::Home;
    case XK_End:
      return K::End;
    case XK_Page_Up:
      return K::PageUp;
    case XK_Page_Down:
      return K::PageDown;
    case XK_Insert:
      return K::Insert;
    case XK_Delete:
      return K::Delete;

    case XK_Escape:
      return K::Escape;
    case XK_Tab:
    case XK_ISO_Left_Tab:
      return K::Tab;
    case XK_Return:
      return K::Enter;
    case XK_BackSpace:
      return K::Backspace;
    case XK_space:
      return K::Space;
    case XK_Print:
      return K::PrintScreen;
    case XK_Pause:
      return K::Pause;
    case XK_Menu:
      return K::Menu;

    case XK_grave:
      return K::Grave;
    case XK_minus:
      return K::Minus;
    case XK_equal:
      return K::Equal;
    case XK_bracketleft:
      return K::LeftBracket;
    case XK_bracketright:
      return K::RightBracket;
    case XK_backslash:
      return K::Backslash;
    case XK_semicolon:
      return K::Semicolon;
    case XK_apostrophe:
      return K::Apostrophe;
    case XK_comma:
      return K::Comma;
    case XK_period:
      return K::Period;
    case XK_slash:
      return K::Slash;

    case XK_KP_0:
      return K::KeypadZero;
    case XK_KP_1:
      return K::KeypadOne;
    case XK_KP_2:
      return K::KeypadTwo;
    case XK_KP_3:
      return K::KeypadThree;
    case XK_KP_4:
      return K::KeypadFour;
    case XK_KP_5:
      return K::KeypadFive;
    case XK_KP_6:
      return K::KeypadSix;
    case XK_KP_7:
      return K::KeypadSeven;
    case XK_KP_8:
      return K::KeypadEight;
    case XK_KP_9:
      return K::KeypadNine;

    case XK_KP_Add:
      return K::KeypadAdd;
    case XK_KP_Subtract:
      return K::KeypadSubtract;
    case XK_KP_Multiply:
      return K::KeypadMultiply;
    case XK_KP_Divide:
      return K::KeypadDivide;
    case XK_KP_Decimal:
      return K::KeypadDecimal;
    case XK_KP_Enter:
      return K::KeypadEnter;

    case XF86XK_AudioRaiseVolume:
      return K::VolumeUp;
    case XF86XK_AudioLowerVolume:
      return K::VolumeDown;
    case XF86XK_AudioMute:
      return K::VolumeMute;
    case XF86XK_AudioPlay:
      return K::MediaPlay;
    case XF86XK_AudioStop:
      return K::MediaStop;
    case XF86XK_AudioPrev:
      return K::MediaPrevious;
    case XF86XK_AudioNext:
      return K::MediaNext;

    default:
      return K::Null;
  }
}

static string VertShader =
    "#version 330 core\n"
    "\n"
    "layout (location=0) in vec2 VertPos;\n"
    "layout (location=1) in vec4 VertColor;\n"
    "\n"
    "uniform vec2 ScreenSize;\n"
    "uniform vec2 Center;\n"
    "uniform float Rotation;\n"
    "\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  vec2 Pos = (VertPos / ScreenSize) * 2.0 - 1.0;\n"
    "  Pos.y = -Pos.y;\n"
    "\n"
    "  vec2 CenterPos = (Center / ScreenSize) * 2.0 - 1.0;\n"
    "  CenterPos.y = -CenterPos.y;\n"
    "\n"
    "  Pos -= CenterPos;\n"
    "\n"
    "  float Angle = radians(Rotation);\n"
    "  float CosA = cos(Angle);\n"
    "  float SinA = sin(Angle);\n"
    "\n"
    "  Pos = mat2(CosA, SinA, -SinA, CosA) * Pos;\n"
    "  Pos += CenterPos;\n"
    "\n"
    "  gl_Position = vec4(Pos, 0.0, 1.0);\n"
    "  FragColor = VertColor;\n"
    "}";

static string FragShader = "#version 330 core\n"
                           "\n"
                           "in vec4 FragColor;\n"
                           "out vec4 ScreenColor;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "  ScreenColor = vec4(FragColor);\n"
                           "}";

static string TextVertShader =
    "#version 330 core\n"
    "\n"
    "layout (location=0) in vec2 VertPos;\n"
    "layout (location=1) in vec2 VertUv;\n"
    "layout (location=2) in vec4 VertColor;\n"
    "\n"
    "uniform vec2 ScreenSize;\n"
    "\n"
    "out vec2 FragUv;\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  vec2 Pos = (VertPos / ScreenSize) * 2.0 - 1.0;\n"
    "  Pos.y = -Pos.y;\n"
    "\n"
    "  gl_Position = vec4(Pos, 0.0, 1.0);\n"
    "  FragUv = VertUv;\n"
    "  FragColor = VertColor;\n"
    "}";

static string TextFragShader =
    "#version 330 core\n"
    "\n"
    "in vec2 FragUv;\n"
    "in vec4 FragColor;\n"
    "\n"
    "uniform sampler2D Atlas;\n"
    "\n"
    "out vec4 ScreenColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  float Coverage = texture(Atlas, FragUv).r;\n"
    "  ScreenColor = vec4(FragColor.rgb, FragColor.a * Coverage);\n"
    "}";

uint make_module(const string &Shader, uint ModuleType)
{
  const char *cShader = Shader.c_str();

  uint ShaderModule = glCreateShader(ModuleType);
  glShaderSource(ShaderModule, 1, &cShader, nullptr);
  glCompileShader(ShaderModule);

  int ShaderStatus;
  glGetShaderiv(ShaderModule, GL_COMPILE_STATUS, &ShaderStatus);

  if (!ShaderStatus)
  {
    char Error[1024];
    glGetShaderInfoLog(ShaderModule, sizeof(Error), nullptr, Error);
    std::cerr << "Shader module compilation error: " << Error << '\n';
    glDeleteShader(ShaderModule);
    std::terminate();
  }

  return ShaderModule;
}

uint make_shader(const string &Vertex, const string &Fragment)
{
  std::array<uint, 2> Modules = { make_module(Vertex, GL_VERTEX_SHADER),
                                  make_module(Fragment, GL_FRAGMENT_SHADER) };

  uint Shader = glCreateProgram();

  for (uint Module : Modules)
    glAttachShader(Shader, Module);

  glLinkProgram(Shader);

  int ShaderStatus;
  glGetProgramiv(Shader, GL_LINK_STATUS, &ShaderStatus);

  if (!ShaderStatus)
  {
    char Error[1024];
    glGetProgramInfoLog(Shader, sizeof(Error), nullptr, Error);
    std::cerr << "Shader linking error: " << Error << '\n';

    for (uint Module : Modules)
      glDeleteShader(Module);

    glDeleteProgram(Shader);
    std::terminate();
  }

  for (uint Module : Modules)
  {
    glDetachShader(Shader, Module);
    glDeleteShader(Module);
  }

  return Shader;
}
} // namespace

namespace marx::graphics
{
Display *get_display()
{
  return Display_;
}

window::window(iVector2 Size, string Name)
{
  Display_ = XOpenDisplay(nullptr);

  if (!Display_)
  {
    std::cerr << "Cannot open display.\n";
    std::terminate();
  }

  int Supported = false;
  XkbSetDetectableAutoRepeat(Display_, true, &Supported);

  Scr_  = DefaultScreen(Display_);
  Root_ = RootWindow(Display_, Scr_);

  int Attributes[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, None };

  Visual_ = glXChooseVisual(Display_, Scr_, Attributes);

  if (!Visual_)
  {
    std::cerr << "Cannot find a GLX-compatible visual.\n";
    XCloseDisplay(Display_);
    Display_ = nullptr;
    std::terminate();
  }

  Colormap_ = XCreateColormap(Display_, Root_, Visual_->visual, AllocNone);

  XSetWindowAttributes xwa{};

  xwa.background_pixel = WhitePixel(Display_, Scr_);
  xwa.border_pixel     = BlackPixel(Display_, Scr_);
  xwa.colormap         = Colormap_;
  xwa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                   ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                   StructureNotifyMask | FocusChangeMask;

  Window_ = XCreateWindow(
      Display_, Root_, 500, 500, Size.x, Size.y, 0, Visual_->depth, InputOutput,
      Visual_->visual, CWBackPixel | CWBorderPixel | CWColormap | CWEventMask,
      &xwa);

  Context_ = glXCreateContext(Display_, Visual_, nullptr, True);

  XFree(Visual_);
  Visual_ = nullptr;

  if (!Context_)
  {
    std::cerr << "Failed to create GLX context.\n";
    XDestroyWindow(Display_, Window_);
    XFreeColormap(Display_, Colormap_);
    XCloseDisplay(Display_);
    Display_  = nullptr;
    Window_   = 0;
    Colormap_ = 0;
    std::terminate();
  }

  if (!glXMakeCurrent(Display_, Window_, Context_))
  {
    std::cerr << "Failed to make GLX context current.\n";
    glXDestroyContext(Display_, Context_);
    Context_ = nullptr;
    XDestroyWindow(Display_, Window_);
    XFreeColormap(Display_, Colormap_);
    XCloseDisplay(Display_);
    Display_  = nullptr;
    Window_   = 0;
    Colormap_ = 0;
    std::terminate();
  }

  if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glXGetProcAddress)))
  {
    std::cerr << "Failed to initialize GLAD.\n";
    glXMakeCurrent(Display_, None, nullptr);
    glXDestroyContext(Display_, Context_);
    Context_ = nullptr;
    XDestroyWindow(Display_, Window_);
    XFreeColormap(Display_, Colormap_);
    XCloseDisplay(Display_);
    Display_  = nullptr;
    Window_   = 0;
    Colormap_ = 0;
    std::terminate();
  }

  SwapInterval_ = reinterpret_cast<PFNGLXSWAPINTERVALEXTPROC>(glXGetProcAddress(
      reinterpret_cast<const GLubyte *>("glXSwapIntervalEXT")));

  enable_vsync(false);

  WM_DELETE_ = XInternAtom(Display_, "WM_DELETE_WINDOW", False);

  XSetWMProtocols(Display_, Window_, &WM_DELETE_, 1);

  set_size(Size);
  set_name(Name);

  XMapWindow(Display_, Window_);
  XFlush(Display_);

  glViewport(0, 0, Size.x, Size.y);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Shader_     = make_shader(VertShader, FragShader);
  TextShader_ = make_shader(TextVertShader, TextFragShader);
  glUseProgram(Shader_);

  glGenVertexArrays(1, &VAO_);
  glGenBuffers(1, &VBO_);

  glBindVertexArray(VAO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);

  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

  constexpr GLsizei Stride = 6 * sizeof(float);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, Stride, nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, Stride,
                        reinterpret_cast<void *>(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &TextVAO_);
  glGenBuffers(1, &TextVBO_);

  glBindVertexArray(TextVAO_);
  glBindBuffer(GL_ARRAY_BUFFER, TextVBO_);

  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

  constexpr GLsizei TextStride = text::FloatsPerVertex * sizeof(float);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, TextStride, nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, TextStride,
                        reinterpret_cast<void *>(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, TextStride,
                        reinterpret_cast<void *>(4 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  Data_.clear();

  Closed_ = false;
}

window::~window()
{
  if (!Closed_)
    close();
}

void window::set_size(iVector2 Size)
{
  if (Display_ && Window_)
  {
    XResizeWindow(Display_, Window_, static_cast<unsigned int>(Size.x),
                  static_cast<unsigned int>(Size.y));
    XFlush(Display_);
  }

  Size_ = Size;
}

iVector2 window::get_size() const
{
  if (Display_ && Window_)
  {
    XWindowAttributes Attributes{};

    if (XGetWindowAttributes(Display_, Window_, &Attributes))
      const_cast<window *>(this)->Size_ = { Attributes.width,
                                            Attributes.height };
  }

  return Size_;
}

void window::set_position(iVector2 Position)
{
  if (Display_ && Window_)
  {
    XMoveWindow(Display_, Window_, Position.x, Position.y);
    XFlush(Display_);
  }

  Position_ = Position;
}

iVector2 window::get_position() const
{
  if (Display_ && Window_)
  {
    Window Child;
    int    x, y;

    if (XTranslateCoordinates(Display_, Window_, Root_, 0, 0, &x, &y, &Child))
      const_cast<window *>(this)->Position_ = { x, y };
  }

  return Position_;
}

void window::set_name(string Name)
{
  if (Display_ && Window_)
  {
    XStoreName(Display_, Window_, Name.c_str());
    XFlush(Display_);
  }

  Name_ = Name;
}

string window::get_name() const
{
  if (Display_ && Window_)
  {
    char *Name = nullptr;

    if (XFetchName(Display_, Window_, &Name) && Name)
    {
      const_cast<window *>(this)->Name_ = Name;
      XFree(Name);
    }
  }

  return Name_;
}

void window::enable_vsync(bool Enable)
{
  if (SwapInterval_)
    SwapInterval_(Display_, Window_, Enable);
}

void window::set_fps_cap(uint fpsCap)
{
  fpsCap_            = fpsCap;
  FrameTimerStarted_ = false;
}

std::unique_ptr<event> window::poll()
{
  if (Closed_ || XPending(Display_) <= 0)
    return nullptr;

  std::unique_ptr<event> Event = std::make_unique<event>();

  XNextEvent(Display_, &Event_);

  switch (Event_.type)
  {
    case ClientMessage:
    {
      if (Event_.xclient.message_type ==
              XInternAtom(Display_, "WM_PROTOCOLS", False) &&
          static_cast<Atom>(Event_.xclient.data.l[0]) == WM_DELETE_)
        Event->set<events::Close>();

      break;
    }
    case DestroyNotify:
      Event->set<events::Close>();
      break;

    case ConfigureNotify:
    {
      XConfigureEvent e = Event_.xconfigure;

      bool Resized = e.width != Size_.x || e.height != Size_.y;
      bool Moved   = e.x != Position_.x || e.y != Position_.y;

      if (Resized)
      {
        Size_ = { e.width, e.height };
        glViewport(0, 0, e.width, e.height);
        Event->set<events::Resized>(e.width, e.height);
        return Event;
      }

      if (Moved)
      {
        Position_ = { e.x, e.y };
        Event->set<events::Moved>(e.x, e.y);
        return Event;
      }

      break;
    }

    case FocusIn:
      Event->set<events::Focused>();
      break;
    case FocusOut:
      Event->set<events::Unfocused>();
      break;

    case KeyPress:
    {
      char   Buffer[32];
      KeySym Symbol;

      XComposeStatus Compose{};
      int Length = XLookupString(&Event_.xkey, Buffer, sizeof(Buffer), &Symbol,
                                 &Compose);

      keyboard::keys Key = x11_to_kb(Symbol);

      char Character = Length == 1 ? Buffer[0] : '\0';

      if (Character == '\r')
        Character = '\n';

      Event->set<events::KeyDown>(Key, Character);
      break;
    }
    case KeyRelease:
    {
      keyboard::keys Key = x11_to_kb(XLookupKeysym(&Event_.xkey, 0));

      Event->set<events::KeyUp>(Key, keyboard::key_to_ascii(Key));
      break;
    }

    case MotionNotify:
      Event->set<events::MouseMove>(static_cast<uint>(Event_.xmotion.x),
                                    static_cast<uint>(Event_.xmotion.y));
      break;
    case ButtonPress:
      Event->set<events::ButtonDown>(Event_.xbutton.button);
      break;
    case ButtonRelease:
      Event->set<events::ButtonUp>(Event_.xbutton.button);
      break;

    default:
      break;
  }

  return Event;
}

void window::clear(color Color) const
{
  glClearColor(Color.r / 255.f, Color.g / 255.f, Color.b / 255.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::draw(const rectangle &Rectangle) const
{
  fVector2 Position = Rectangle.get_position();
  fVector2 Size     = Rectangle.get_size();
  color    Color    = Rectangle.get_color();
  float    Rotation = Rectangle.get_rotation();

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

  float centerX = Position.x + Size.x / 2.0f;
  float centerY = Position.y + Size.y / 2.0f;

  GLint CenterLocation = glGetUniformLocation(Shader_, "Center");
  glUniform2f(CenterLocation, centerX, centerY);

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

    Data.insert(Data.end(), { Center.x, Center.y, r, g, b, a, P0.x, P0.y, r, g,
                              b, a, P1.x, P1.y, r, g, b, a });
  }

  glUseProgram(Shader_);

  glUniform2f(glGetUniformLocation(Shader_, "ScreenSize"),
              static_cast<float>(Size_.x), static_cast<float>(Size_.y));

  glUniform1f(glGetUniformLocation(Shader_, "Rotation"), 0.f);

  glUniform2f(glGetUniformLocation(Shader_, "Center"), Center.x, Center.y);

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

void window::close()
{
  if (Closed_)
    return;

  if (Context_)
  {
    if (glXGetCurrentContext() != Context_)
      glXMakeCurrent(Display_, Window_, Context_);

    glDeleteBuffers(1, &VBO_);
    glDeleteVertexArrays(1, &VAO_);
    glDeleteProgram(Shader_);

    glDeleteBuffers(1, &TextVBO_);
    glDeleteVertexArrays(1, &TextVAO_);
    glDeleteProgram(TextShader_);

    VBO_        = 0;
    VAO_        = 0;
    Shader_     = 0;
    TextVBO_    = 0;
    TextVAO_    = 0;
    TextShader_ = 0;

    glXMakeCurrent(Display_, None, nullptr);
    glXDestroyContext(Display_, Context_);
    Context_ = nullptr;
  }

  if (Display_ && Window_)
  {
    XUnmapWindow(Display_, Window_);
    XDestroyWindow(Display_, Window_);
  }

  if (Display_ && Colormap_)
    XFreeColormap(Display_, Colormap_);

  if (Display_)
    XCloseDisplay(Display_);

  Display_  = nullptr;
  Window_   = 0;
  Colormap_ = 0;
  Closed_   = true;
}

bool window::is_open() const
{
  return !Closed_;
}

} // namespace marx::graphics

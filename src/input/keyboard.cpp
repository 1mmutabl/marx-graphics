#include "marx/graphics-internal/input/keyboard.hpp"

#include <X11/X.h>
#include <X11/XF86keysym.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <sys/types.h>

#include "marx/graphics-internal/window.hpp"

namespace marx::graphics
{
namespace
{
KeySym key_to_x11(keyboard::keys Key)
{
  using K = keyboard::keys;

  switch (Key)
  {
    case K::A:
      return XK_A;
    case K::B:
      return XK_B;
    case K::C:
      return XK_C;
    case K::D:
      return XK_D;
    case K::E:
      return XK_E;
    case K::F:
      return XK_F;
    case K::G:
      return XK_G;
    case K::H:
      return XK_H;
    case K::I:
      return XK_I;
    case K::J:
      return XK_J;
    case K::K:
      return XK_K;
    case K::L:
      return XK_L;
    case K::M:
      return XK_M;
    case K::N:
      return XK_N;
    case K::O:
      return XK_O;
    case K::P:
      return XK_P;
    case K::Q:
      return XK_Q;
    case K::R:
      return XK_R;
    case K::S:
      return XK_S;
    case K::T:
      return XK_T;
    case K::U:
      return XK_U;
    case K::V:
      return XK_V;
    case K::W:
      return XK_W;
    case K::X:
      return XK_X;
    case K::Y:
      return XK_Y;
    case K::Z:
      return XK_Z;

    case K::Zero:
      return XK_0;
    case K::One:
      return XK_1;
    case K::Two:
      return XK_2;
    case K::Three:
      return XK_3;
    case K::Four:
      return XK_4;
    case K::Five:
      return XK_5;
    case K::Six:
      return XK_6;
    case K::Seven:
      return XK_7;
    case K::Eight:
      return XK_8;
    case K::Nine:
      return XK_9;

    case K::F1:
      return XK_F1;
    case K::F2:
      return XK_F2;
    case K::F3:
      return XK_F3;
    case K::F4:
      return XK_F4;
    case K::F5:
      return XK_F5;
    case K::F6:
      return XK_F6;
    case K::F7:
      return XK_F7;
    case K::F8:
      return XK_F8;
    case K::F9:
      return XK_F9;
    case K::F10:
      return XK_F10;
    case K::F11:
      return XK_F11;
    case K::F12:
      return XK_F12;
    case K::F13:
      return XK_F13;
    case K::F14:
      return XK_F14;
    case K::F15:
      return XK_F15;
    case K::F16:
      return XK_F16;
    case K::F17:
      return XK_F17;
    case K::F18:
      return XK_F18;
    case K::F19:
      return XK_F19;
    case K::F20:
      return XK_F20;
    case K::F21:
      return XK_F21;
    case K::F22:
      return XK_F22;
    case K::F23:
      return XK_F23;
    case K::F24:
      return XK_F24;

    case K::LeftShift:
      return XK_Shift_L;
    case K::RightShift:
      return XK_Shift_R;
    case K::LeftControl:
      return XK_Control_L;
    case K::RightControl:
      return XK_Control_R;
    case K::LeftAlt:
      return XK_Alt_L;
    case K::RightAlt:
      return XK_Alt_R;
    case K::LeftSuper:
      return XK_Super_L;
    case K::RightSuper:
      return XK_Super_R;
    case K::CapsLock:
      return XK_Caps_Lock;
    case K::NumLock:
      return XK_Num_Lock;
    case K::ScrollLock:
      return XK_Scroll_Lock;

    case K::Up:
      return XK_Up;
    case K::Down:
      return XK_Down;
    case K::Left:
      return XK_Left;
    case K::Right:
      return XK_Right;
    case K::Home:
      return XK_Home;
    case K::End:
      return XK_End;
    case K::PageUp:
      return XK_Page_Up;
    case K::PageDown:
      return XK_Page_Down;
    case K::Insert:
      return XK_Insert;
    case K::Delete:
      return XK_Delete;

    case K::Escape:
      return XK_Escape;
    case K::Tab:
      return XK_Tab;
    case K::Enter:
      return XK_Return;
    case K::Backspace:
      return XK_BackSpace;
    case K::Space:
      return XK_space;
    case K::PrintScreen:
      return XK_Print;
    case K::Pause:
      return XK_Pause;
    case K::Menu:
      return XK_Menu;

    case K::Grave:
      return XK_grave;
    case K::Minus:
      return XK_minus;
    case K::Equal:
      return XK_equal;
    case K::LeftBracket:
      return XK_bracketleft;
    case K::RightBracket:
      return XK_bracketright;
    case K::Backslash:
      return XK_backslash;
    case K::Semicolon:
      return XK_semicolon;
    case K::Apostrophe:
      return XK_apostrophe;
    case K::Comma:
      return XK_comma;
    case K::Period:
      return XK_period;
    case K::Slash:
      return XK_slash;

    case K::KeypadZero:
      return XK_KP_0;
    case K::KeypadOne:
      return XK_KP_1;
    case K::KeypadTwo:
      return XK_KP_2;
    case K::KeypadThree:
      return XK_KP_3;
    case K::KeypadFour:
      return XK_KP_4;
    case K::KeypadFive:
      return XK_KP_5;
    case K::KeypadSix:
      return XK_KP_6;
    case K::KeypadSeven:
      return XK_KP_7;
    case K::KeypadEight:
      return XK_KP_8;
    case K::KeypadNine:
      return XK_KP_9;

    case K::KeypadAdd:
      return XK_KP_Add;
    case K::KeypadSubtract:
      return XK_KP_Subtract;
    case K::KeypadMultiply:
      return XK_KP_Multiply;
    case K::KeypadDivide:
      return XK_KP_Divide;
    case K::KeypadDecimal:
      return XK_KP_Decimal;
    case K::KeypadEnter:
      return XK_KP_Enter;

    case K::VolumeUp:
      return XF86XK_AudioRaiseVolume;
    case K::VolumeDown:
      return XF86XK_AudioLowerVolume;
    case K::VolumeMute:
      return XF86XK_AudioMute;
    case K::MediaPlay:
      return XF86XK_AudioPlay;
    case K::MediaStop:
      return XF86XK_AudioStop;
    case K::MediaPrevious:
      return XF86XK_AudioPrev;
    case K::MediaNext:
      return XF86XK_AudioNext;

    case K::Null:
    default:
      return NoSymbol;
  }
}
} // namespace

char keyboard::key_to_ascii(keys Key)
{
  using K = keys;

  switch (Key)
  {
    case K::A:
      return 'A';
    case K::B:
      return 'B';
    case K::C:
      return 'C';
    case K::D:
      return 'D';
    case K::E:
      return 'E';
    case K::F:
      return 'F';
    case K::G:
      return 'G';
    case K::H:
      return 'H';
    case K::I:
      return 'I';
    case K::J:
      return 'J';
    case K::K:
      return 'K';
    case K::L:
      return 'L';
    case K::M:
      return 'M';
    case K::N:
      return 'N';
    case K::O:
      return 'O';
    case K::P:
      return 'P';
    case K::Q:
      return 'Q';
    case K::R:
      return 'R';
    case K::S:
      return 'S';
    case K::T:
      return 'T';
    case K::U:
      return 'U';
    case K::V:
      return 'V';
    case K::W:
      return 'W';
    case K::X:
      return 'X';
    case K::Y:
      return 'Y';
    case K::Z:
      return 'Z';

    case K::Zero:
      return '0';
    case K::One:
      return '1';
    case K::Two:
      return '2';
    case K::Three:
      return '3';
    case K::Four:
      return '4';
    case K::Five:
      return '5';
    case K::Six:
      return '6';
    case K::Seven:
      return '7';
    case K::Eight:
      return '8';
    case K::Nine:
      return '9';

    case K::Backspace:
      return '\b';
    case K::Tab:
      return '\t';
    case K::Enter:
      return '\n';
    case K::Escape:
      return '\x1B';
    case K::Delete:
      return '\x7F';
    case K::Space:
      return ' ';

    case K::Apostrophe:
      return '\'';
    case K::Comma:
      return ',';
    case K::Minus:
      return '-';
    case K::Period:
      return '.';
    case K::Slash:
      return '/';
    case K::Semicolon:
      return ';';
    case K::Equal:
      return '=';
    case K::LeftBracket:
      return '[';
    case K::Backslash:
      return '\\';
    case K::RightBracket:
      return ']';
    case K::Grave:
      return '`';

    default:
      return '\0';
  }
}

bool keyboard::key_down(keyboard::keys Key)
{
  Display *Display_ = get_display();

  if (!Display_)
    return false;

  KeySym xKey = key_to_x11(Key);

  char Keys[32];
  XQueryKeymap(Display_, Keys);

  KeyCode Keycode = XKeysymToKeycode(Display_, xKey);
  return (Keys[Keycode >> 3] & (1 << (Keycode & 7))) != 0;
}
} // namespace marx::graphics

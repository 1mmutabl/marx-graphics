#include <string>

#include "internal.hpp"

namespace marx::graphics::internal
{
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
        case XK_KP_Insert:
            return K::KeypadZero;
        case XK_KP_1:
        case XK_KP_End:
            return K::KeypadOne;
        case XK_KP_2:
        case XK_KP_Down:
            return K::KeypadTwo;
        case XK_KP_3:
        case XK_KP_Next:
            return K::KeypadThree;
        case XK_KP_4:
        case XK_KP_Left:
            return K::KeypadFour;
        case XK_KP_5:
        case XK_KP_Begin:
            return K::KeypadFive;
        case XK_KP_6:
        case XK_KP_Right:
            return K::KeypadSix;
        case XK_KP_7:
        case XK_KP_Home:
            return K::KeypadSeven;
        case XK_KP_8:
        case XK_KP_Up:
            return K::KeypadEight;
        case XK_KP_9:
        case XK_KP_Prior:
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
        case XK_KP_Delete:
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

keyboard::keys get_key(XKeyEvent &Event)
{
    char   Buffer[32];
    KeySym Symbol = NoSymbol;

    XLookupString(&Event, Buffer, sizeof(Buffer), &Symbol, nullptr);

    KeySym Lower, Upper;
    XConvertCase(Symbol, &Lower, &Upper);

    keyboard::keys Key = x11_to_kb(Lower);

    if (Key == keyboard::keys::Null)
        Key = x11_to_kb(XLookupKeysym(&Event, 0));

    return Key;
}

string get_text(XKeyEvent &Event)
{
    char   Buffer[64];
    KeySym Symbol;

    if (InputContext_)
    {
        Status LookupStatus = 0;

        const int Length =
            Xutf8LookupString(InputContext_, &Event, Buffer, sizeof(Buffer),
                              &Symbol, &LookupStatus);

        if (LookupStatus != XLookupChars && LookupStatus != XLookupBoth)
            return "";

        return string(Buffer, Length);
    }

    const int Length =
        XLookupString(&Event, Buffer, sizeof(Buffer), &Symbol, nullptr);

    string Text;

    for (int Index = 0; Index < Length; ++Index)
        Text += utf8::encode(static_cast<unsigned char>(Buffer[Index]));

    return Text;
}

bool is_printable(const string &Text)
{
    if (Text.empty())
        return false;

    const unsigned char First = Text[0];
    return First >= 0x20 && First != 0x7F;
}

keyboard::modifiers get_modifiers(unsigned int State)
{
    keyboard::modifiers Modifiers;

    Modifiers.Shift   = (State & ShiftMask) != 0;
    Modifiers.Control = (State & ControlMask) != 0;
    Modifiers.Alt     = (State & Mod1Mask) != 0;
    Modifiers.Super   = (State & Mod4Mask) != 0;

    return Modifiers;
}

mouse::buttons x11_to_mouse(unsigned int Button)
{
    switch (Button)
    {
        case Button1:
            return mouse::buttons::Left;
        case Button2:
            return mouse::buttons::Middle;
        case Button3:
            return mouse::buttons::Right;
        default:
            return mouse::buttons::Null;
    }
}
} // namespace marx::graphics::internal

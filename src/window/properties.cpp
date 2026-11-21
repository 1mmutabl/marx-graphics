#include "marx/graphics-internal/window.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

#include "internal.hpp"

using namespace marx::graphics::internal;

namespace marx::graphics
{
void window::set_icon(const texture &Texture)
{
    int                  Width    = Texture.get_width();
    int                  Height   = Texture.get_height();
    int                  Channels = Texture.get_channels();
    std::vector<uint8_t> Pixels   = Texture.get_pixels();

    if (Channels != 4)
    {
        std::cerr << "Expected image with 4 channels, got " << Channels
                  << " channels." << '\n';
        return;
    }

    std::vector<unsigned long> Icon(2 + Width * Height);

    if (!Icon.data())
        return;

    Icon[0] = Width;
    Icon[1] = Height;

    for (int i = 0; i < Width * Height; i++)
    {
        uint8_t r = Pixels.at(i * 4 + 0);
        uint8_t g = Pixels.at(i * 4 + 1);
        uint8_t b = Pixels.at(i * 4 + 2);
        uint8_t a = Pixels.at(i * 4 + 3);

        Icon[i + 2] = (static_cast<unsigned long>(a << 24) |
                       static_cast<unsigned long>(r << 16) |
                       static_cast<unsigned long>(g << 8) |
                       static_cast<unsigned long>(b));
    }

    Atom NetWmIcon = XInternAtom(Display_, "_NET_WM_ICON", false);
    Atom Cardinal  = XInternAtom(Display_, "CARDINAL", false);

    XChangeProperty(Display_, Window_, NetWmIcon, Cardinal, 32, PropModeReplace,
                    reinterpret_cast<uint8_t *>(Icon.data()),
                    2 + Width * Height);

    XFlush(Display_);
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

void window::set_min_size(iVector2 Size)
{
    MinSize_ = Size;
    apply_size_hints();
}

void window::set_max_size(iVector2 Size)
{
    MaxSize_ = Size;
    apply_size_hints();
}

void window::set_resizable(bool Resizable)
{
    if (Resizable)
    {
        MinSize_ = { 0, 0 };
        MaxSize_ = { 0, 0 };
    }
    else
    {
        MinSize_ = get_size();
        MaxSize_ = MinSize_;
    }

    apply_size_hints();
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

        if (XTranslateCoordinates(Display_, Window_, Root_, 0, 0, &x, &y,
                                  &Child))
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

void window::set_fullscreen(bool Fullscreen)
{
    if (!Display_ || !Window_)
        return;

    XEvent Message{};

    Message.xclient.type   = ClientMessage;
    Message.xclient.window = Window_;
    Message.xclient.message_type =
        XInternAtom(Display_, "_NET_WM_STATE", False);
    Message.xclient.format    = 32;
    Message.xclient.data.l[0] = Fullscreen ? 1 : 0;
    Message.xclient.data.l[1] =
        XInternAtom(Display_, "_NET_WM_STATE_FULLSCREEN", False);

    XSendEvent(Display_, Root_, False,
               SubstructureRedirectMask | SubstructureNotifyMask, &Message);
    XFlush(Display_);
}

bool window::is_fullscreen() const
{
    if (!Display_ || !Window_)
        return false;

    const Atom State = XInternAtom(Display_, "_NET_WM_STATE", False);
    const Atom Fullscreen =
        XInternAtom(Display_, "_NET_WM_STATE_FULLSCREEN", False);

    Atom           Type;
    int            Format;
    unsigned long  Count, After;
    unsigned char *Data = nullptr;

    if (XGetWindowProperty(Display_, Window_, State, 0, 1024, False, XA_ATOM,
                           &Type, &Format, &Count, &After, &Data) != Success ||
        !Data)
        return false;

    const Atom *Atoms = reinterpret_cast<const Atom *>(Data);
    bool        Found = false;

    for (unsigned long Index = 0; Index < Count; ++Index)
        if (Atoms[Index] == Fullscreen)
            Found = true;

    XFree(Data);

    return Found;
}

bool window::is_focused() const
{
    return Focused_;
}

void window::show()
{
    if (!Display_ || !Window_)
        return;

    XMapWindow(Display_, Window_);
    XFlush(Display_);
}

void window::hide()
{
    if (!Display_ || !Window_)
        return;

    XUnmapWindow(Display_, Window_);
    XFlush(Display_);
}

bool window::is_visible() const
{
    if (!Display_ || !Window_)
        return false;

    XWindowAttributes Attributes{};

    if (!XGetWindowAttributes(Display_, Window_, &Attributes))
        return false;

    return Attributes.map_state != IsUnmapped;
}

iVector2 window::get_mouse_position() const
{
    if (!Display_ || !Window_)
        return { 0, 0 };

    Window       Root, Child;
    int          RootX, RootY, WindowX, WindowY;
    unsigned int Mask;

    XQueryPointer(Display_, Window_, &Root, &Child, &RootX, &RootY, &WindowX,
                  &WindowY, &Mask);

    return { WindowX, WindowY };
}

void window::set_cursor(cursors Shape)
{
    if (Shape == CurrentCursor_ && CursorApplied_)
        return;

    CurrentCursor_ = Shape;
    apply_cursor();
}

void window::set_cursor_visible(bool Visible)
{
    if (Visible == CursorVisible_ && CursorApplied_)
        return;

    CursorVisible_ = Visible;
    apply_cursor();
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
} // namespace marx::graphics

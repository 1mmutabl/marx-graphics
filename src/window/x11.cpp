#include <cstddef>
#include <vector>

#include "internal.hpp"

namespace marx::graphics::internal
{
namespace
{
unsigned int get_cursor_shape(marx::graphics::cursors Shape)
{
    using C = marx::graphics::cursors;

    switch (Shape)
    {
        case C::Text:
            return XC_xterm;
        case C::Hand:
            return XC_hand2;
        case C::Crosshair:
            return XC_crosshair;
        case C::Wait:
            return XC_watch;
        case C::Move:
            return XC_fleur;
        case C::ResizeHorizontal:
            return XC_sb_h_double_arrow;
        case C::ResizeVertical:
            return XC_sb_v_double_arrow;
        default:
            return XC_left_ptr;
    }
}

Cursor get_cursor(marx::graphics::cursors Shape)
{
    Cursor &Created = Cursors_[static_cast<std::size_t>(Shape)];

    if (!Created)
        Created = XCreateFontCursor(Display_, get_cursor_shape(Shape));

    return Created;
}

Cursor get_blank_cursor()
{
    if (BlankCursor_)
        return BlankCursor_;

    const char Data[1] = { 0 };
    XColor     Black{};

    Pixmap Bitmap = XCreateBitmapFromData(Display_, Window_, Data, 1, 1);

    BlankCursor_ =
        XCreatePixmapCursor(Display_, Bitmap, Bitmap, &Black, &Black, 0, 0);

    XFreePixmap(Display_, Bitmap);

    return BlankCursor_;
}
} // namespace

void apply_cursor()
{
    if (!Display_ || !Window_)
        return;

    XDefineCursor(Display_, Window_,
                  CursorVisible_ ? get_cursor(CurrentCursor_)
                                 : get_blank_cursor());
    XFlush(Display_);

    CursorApplied_ = true;
}

void apply_size_hints()
{
    if (!Display_ || !Window_)
        return;

    XSizeHints Hints{};
    long       Supplied = 0;

    XGetWMNormalHints(Display_, Window_, &Hints, &Supplied);

    if (MinSize_.x > 0 && MinSize_.y > 0)
    {
        Hints.flags |= PMinSize;
        Hints.min_width  = MinSize_.x;
        Hints.min_height = MinSize_.y;
    }
    else
        Hints.flags &= ~PMinSize;

    if (MaxSize_.x > 0 && MaxSize_.y > 0)
    {
        Hints.flags |= PMaxSize;
        Hints.max_width  = MaxSize_.x;
        Hints.max_height = MaxSize_.y;
    }
    else
        Hints.flags &= ~PMaxSize;

    XSetWMNormalHints(Display_, Window_, &Hints);
    XFlush(Display_);
}

XVisualInfo *choose_visual(bool Alpha, bool Multisample)
{
    if (!Alpha)
    {
        std::vector<int> Attributes = { GLX_RGBA, GLX_DOUBLEBUFFER,
                                        GLX_DEPTH_SIZE, 24 };

        if (Multisample)
            Attributes.insert(Attributes.end(),
                              { GLX_SAMPLE_BUFFERS, 1, GLX_SAMPLES, 4 });

        Attributes.push_back(None);

        return glXChooseVisual(Display_, Scr_, Attributes.data());
    }

    std::vector<int> Attributes = { GLX_X_RENDERABLE,  True,
                                    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
                                    GLX_RENDER_TYPE,   GLX_RGBA_BIT,
                                    GLX_DOUBLEBUFFER,  True,
                                    GLX_DEPTH_SIZE,    24,
                                    GLX_ALPHA_SIZE,    8 };

    if (Multisample)
        Attributes.insert(Attributes.end(),
                          { GLX_SAMPLE_BUFFERS, 1, GLX_SAMPLES, 4 });

    Attributes.push_back(None);

    int          Count   = 0;
    GLXFBConfig *Configs =
        glXChooseFBConfig(Display_, Scr_, Attributes.data(), &Count);

    if (!Configs)
        return nullptr;

    XVisualInfo *Chosen = nullptr;

    for (int Index = 0; Index < Count && !Chosen; ++Index)
    {
        XVisualInfo *Candidate =
            glXGetVisualFromFBConfig(Display_, Configs[Index]);

        if (!Candidate)
            continue;

        if (Candidate->depth == 32)
            Chosen = Candidate;
        else
            XFree(Candidate);
    }

    XFree(Configs);

    return Chosen;
}

void apply_config(const marx::graphics::window_config &Config,
                  marx::iVector2 Size, marx::iVector2 Position)
{
    using marx::graphics::config;

    if (Config.has(config::Centered))
    {
        XSizeHints Hints{};

        Hints.flags = USPosition | PPosition;
        Hints.x     = Position.x;
        Hints.y     = Position.y;

        XSetWMNormalHints(Display_, Window_, &Hints);
    }

    if (Config.has(config::Fixed))
    {
        MinSize_ = Size;
        MaxSize_ = Size;
        apply_size_hints();
    }

    if (Config.has(config::Undecorated))
    {
        const Atom MotifHints = XInternAtom(Display_, "_MOTIF_WM_HINTS", False);

        const long Hints[5] = { 2, 0, 0, 0, 0 };

        XChangeProperty(Display_, Window_, MotifHints, MotifHints, 32,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char *>(Hints), 5);
    }

    std::vector<Atom> States;

    if (Config.has(config::Maximized))
    {
        States.push_back(
            XInternAtom(Display_, "_NET_WM_STATE_MAXIMIZED_VERT", False));
        States.push_back(
            XInternAtom(Display_, "_NET_WM_STATE_MAXIMIZED_HORZ", False));
    }

    if (Config.has(config::Fullscreen))
        States.push_back(
            XInternAtom(Display_, "_NET_WM_STATE_FULLSCREEN", False));

    if (Config.has(config::Floating))
        States.push_back(XInternAtom(Display_, "_NET_WM_STATE_ABOVE", False));

    if (!States.empty())
        XChangeProperty(Display_, Window_,
                        XInternAtom(Display_, "_NET_WM_STATE", False), XA_ATOM,
                        32, PropModeReplace,
                        reinterpret_cast<const unsigned char *>(States.data()),
                        static_cast<int>(States.size()));

    XWMHints Hints{};

    Hints.flags = InputHint | StateHint;
    Hints.input = True;
    Hints.initial_state =
        Config.has(config::Minimized) ? IconicState : NormalState;

    XSetWMHints(Display_, Window_, &Hints);
}
} // namespace marx::graphics::internal

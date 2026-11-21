#include "marx/graphics-internal/window.hpp"

#include <glad/gl.h>

#include <memory>
#include <utility>

#include "internal.hpp"

using namespace marx::graphics::internal;

namespace
{
XEvent Event_;
} // namespace

namespace marx::graphics
{
std::unique_ptr<event> window::poll()
{
    if (Closed_)
        return nullptr;

    if (!Pending_.empty())
    {
        std::unique_ptr<event> Next = std::move(Pending_.front());
        Pending_.pop_front();
        return Next;
    }

    if (XPending(Display_) <= 0)
        return nullptr;

    std::unique_ptr<event> Event = std::make_unique<event>();

    XNextEvent(Display_, &Event_);

    if (Event_.type == KeyRelease)
        HeldKeys_[Event_.xkey.keycode] = false;

    if (XFilterEvent(&Event_, None))
        return Event;

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

            bool Resized =
                e.width != ReportedSize_.x || e.height != ReportedSize_.y;
            bool Moved =
                e.x != ReportedPosition_.x || e.y != ReportedPosition_.y;

            if (Resized)
            {
                ReportedSize_ = { e.width, e.height };
                Size_         = ReportedSize_;
                glViewport(0, 0, e.width, e.height);
                Event->set<events::Resized>(e.width, e.height);
                return Event;
            }

            if (Moved)
            {
                ReportedPosition_ = { e.x, e.y };
                Position_         = ReportedPosition_;
                Event->set<events::Moved>(e.x, e.y);
                return Event;
            }

            break;
        }

        case FocusIn:
            Focused_ = true;

            if (InputContext_)
                XSetICFocus(InputContext_);

            Event->set<events::Focused>();
            break;
        case FocusOut:
            Focused_ = false;
            HeldKeys_.fill(false);

            if (InputContext_)
                XUnsetICFocus(InputContext_);

            Event->set<events::Unfocused>();
            break;

        case KeyPress:
        {
            XKeyEvent &Press = Event_.xkey;

            const keyboard::keys      Key       = get_key(Press);
            const keyboard::modifiers Modifiers = get_modifiers(Press.state);

            const bool Repeat = Press.keycode != 0 && HeldKeys_[Press.keycode];

            if (Press.keycode != 0)
                HeldKeys_[Press.keycode] = true;

            const string Text = get_text(Press);

            char Character = Text.size() == 1 ? Text[0] : '\0';

            if (Character == '\r')
                Character = '\n';

            Event->set<events::KeyDown>(Key, Character, Modifiers, Repeat);

            if (is_printable(Text) && !Modifiers.Control && !Modifiers.Alt &&
                !Modifiers.Super)
            {
                Pending_.push_back(std::make_unique<event>());
                Pending_.back()->set<events::TextInput>(Text);
            }

            break;
        }
        case KeyRelease:
        {
            XKeyEvent &Release = Event_.xkey;

            const keyboard::keys Key = get_key(Release);

            Event->set<events::KeyUp>(Key, keyboard::key_to_ascii(Key),
                                      get_modifiers(Release.state));
            break;
        }

        case MotionNotify:
            Event->set<events::MouseMove>(Event_.xmotion.x, Event_.xmotion.y);
            break;

        case EnterNotify:
            if (Event_.xcrossing.mode == NotifyNormal)
                Event->set<events::MouseEntered>(Event_.xcrossing.x,
                                                 Event_.xcrossing.y);
            break;
        case LeaveNotify:
            if (Event_.xcrossing.mode == NotifyNormal)
                Event->set<events::MouseLeft>(Event_.xcrossing.x,
                                              Event_.xcrossing.y);
            break;

        case ButtonPress:
        {
            XButtonEvent &Press = Event_.xbutton;

            const keyboard::modifiers Modifiers = get_modifiers(Press.state);

            if (Press.button >= Button4 && Press.button <= 7)
            {
                float DeltaX = 0.f;
                float DeltaY = 0.f;

                if (Press.button == Button4)
                    DeltaY = 1.f;
                else if (Press.button == Button5)
                    DeltaY = -1.f;
                else if (Press.button == 6)
                    DeltaX = -1.f;
                else
                    DeltaX = 1.f;

                Event->set<events::Scrolled>(Press.x, Press.y, DeltaX, DeltaY,
                                             Modifiers);
                break;
            }

            const mouse::buttons Button = x11_to_mouse(Press.button);

            if (Button != mouse::buttons::Null)
                Event->set<events::ButtonDown>(Button, Press.x, Press.y,
                                               Modifiers);

            break;
        }
        case ButtonRelease:
        {
            XButtonEvent &Release = Event_.xbutton;

            const mouse::buttons Button = x11_to_mouse(Release.button);

            if (Button != mouse::buttons::Null)
                Event->set<events::ButtonUp>(Button, Release.x, Release.y,
                                             get_modifiers(Release.state));

            break;
        }

        default:
            break;
    }

    return Event;
}
} // namespace marx::graphics

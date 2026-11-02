#include "marx/graphics-internal/input/mouse.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>

#include "marx/graphics-internal/window.hpp"

namespace marx::graphics
{
bool mouse::button_down(mouse::buttons Button)
{
  Display *Display_ = get_display();

  if (!Display_)
    return false;

  Window       Root, Child;
  int          RootX, RootY, WindowX, WindowY;
  unsigned int Mask;

  XQueryPointer(Display_, DefaultRootWindow(Display_), &Root, &Child, &RootX,
                &RootY, &WindowX, &WindowY, &Mask);

  switch (Button)
  {
    case mouse::buttons::Left:
      return (Mask & Button1Mask) != 0;
    case mouse::buttons::Middle:
      return (Mask & Button2Mask) != 0;
    case mouse::buttons::Right:
      return (Mask & Button3Mask) != 0;
    default:
      return false;
  }
}
} // namespace marx::graphics

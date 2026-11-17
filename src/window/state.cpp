#include <array>
#include <deque>
#include <memory>

#include "internal.hpp"
#include "marx/graphics-internal/event.hpp"
#include "marx/graphics-internal/vector.hpp"
#include "marx/graphics-internal/window.hpp"

namespace marx::graphics::internal
{
Display *Display_   = nullptr;
int      Scr_       = 0;
Window   Root_      = 0;
Window   Window_    = 0;
Atom     WM_DELETE_ = 0;

GLXContext Context_ = nullptr;

XIM InputMethod_  = nullptr;
XIC InputContext_ = nullptr;

PFNGLXSWAPINTERVALEXTPROC SwapInterval_ = nullptr;

std::deque<std::unique_ptr<event>> Pending_;
std::array<bool, 256>              HeldKeys_{};
bool                               Focused_     = false;
bool                               Transparent_ = false;

iVector2 MinSize_;
iVector2 MaxSize_;

iVector2 ReportedSize_;
iVector2 ReportedPosition_;

std::array<Cursor, 8> Cursors_{};
Cursor                BlankCursor_   = 0;
cursors               CurrentCursor_ = cursors::Arrow;
bool                  CursorVisible_ = true;
bool                  CursorApplied_ = false;
} // namespace marx::graphics::internal

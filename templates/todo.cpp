/*
 * Template 00 — To do
 *
 * File:        todo.cpp
 * Author:      1mmutabl
 * Created:     2026-10-27
 * Updated:     2026-10-30
 *
 * Description:
 *   Creates a simple to do app with all accumulated knowledge.
 *
 * NOTE: Because i only have 1 file, this will get a bit messy.
 */

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <format>
#include <limits>
#include <string>
#include <vector>

#include "marx/graphics.hpp"

namespace graphics = marx::graphics;
namespace events   = graphics::events;
namespace keyboard = graphics::keyboard;
namespace mouse    = graphics::mouse;
namespace utf8     = graphics::utf8;

namespace
{
constexpr const char *FontPath = "resources/childer/Childer.otf";

constexpr float Margin       = 24.f;
constexpr float HeaderHeight = 96.f;
constexpr float ListGap      = 16.f;
constexpr float RowHeight    = 52.f;
constexpr float RowGap       = 8.f;
constexpr float FooterHeight = 56.f;
constexpr float Padding      = 16.f;
constexpr float CheckRadius  = 13.f;
constexpr float IconSize     = 28.f;
constexpr float ScrollStep   = 48.f;

constexpr std::size_t MaxLength = 280;

constexpr float ListTop     = HeaderHeight + RowHeight + ListGap;
constexpr float LabelOffset = Padding * 2.f + CheckRadius * 2.f;
constexpr float IconReserve = Padding * 2.f + IconSize;

constexpr marx::fVector2 IconBox(IconSize, IconSize);
constexpr marx::fVector2 ClearSize(130.f, 34.f);

const graphics::color Background(24, 24, 30, 255);
const graphics::color RowColor(38, 38, 48, 255);
const graphics::color HoverColor(52, 52, 66, 255);
const graphics::color Accent(110, 200, 140, 255);
const graphics::color AccentHover(145, 225, 170, 255);
const graphics::color Danger(235, 95, 95, 255);
const graphics::color TextColor(235, 235, 240, 255);
const graphics::color DimColor(130, 130, 148, 255);

struct task
{
  graphics::text Label;
  bool           Done = false;
};

struct row_box
{
  float Top;
  float Height;
};

bool contains(marx::fVector2 Position, marx::fVector2 Size,
              marx::fVector2 Point)
{
  return Point >= Position && Point <= Position + Size;
}

marx::fVector2 get_window_size(const graphics::window &Window)
{
  const marx::iVector2 Size = Window.get_size();
  return { static_cast<float>(Size.x), static_cast<float>(Size.y) };
}

float get_row_width(marx::fVector2 WindowSize)
{
  return WindowSize.x - Margin * 2.f;
}

float get_list_bottom(marx::fVector2 WindowSize)
{
  return WindowSize.y - FooterHeight;
}

float get_max_label_width(marx::fVector2 WindowSize)
{
  return get_row_width(WindowSize) - LabelOffset - IconReserve;
}

marx::fVector2 get_clear_position(marx::fVector2 WindowSize)
{
  return { WindowSize.x - Margin - ClearSize.x,
           get_list_bottom(WindowSize) + (FooterHeight - ClearSize.y) / 2.f };
}

bool in_list(marx::fVector2 Point, marx::fVector2 WindowSize)
{
  return Point.y >= ListTop && Point.y <= get_list_bottom(WindowSize);
}

marx::fVector2 get_row_position(const row_box &Box, float ScrollOffset)
{
  return { Margin, ListTop + Box.Top - ScrollOffset };
}

marx::fVector2 get_icon_position(marx::fVector2 RowPosition, float RowWidth)
{
  return { RowPosition.x + RowWidth - Padding - IconSize,
           RowPosition.y + (RowHeight - IconSize) / 2.f };
}

std::string trim(const std::string &String)
{
  const std::size_t First = String.find_first_not_of(' ');

  if (First == std::string::npos)
    return "";

  const std::size_t Last = String.find_last_not_of(' ');
  return String.substr(First, Last - First + 1);
}

void place(graphics::text &Label, marx::fVector2 Position)
{
  if (Label.get_position() != Position)
    Label.set_position(Position);
}

void relabel(graphics::text &Label, const std::string &String)
{
  if (Label.get_string() != String)
    Label.set_string(String);
}

void toggle_task(task &Task)
{
  Task.Done = !Task.Done;
  Task.Label.set_color(Task.Done ? DimColor : TextColor);
}

void insert_text(graphics::text &InputText, std::size_t &Caret,
                 const std::string &Text)
{
  const std::size_t Length = utf8::length(Text);

  if (InputText.get_length() + Length > MaxLength)
    return;

  std::string Current = InputText.get_string();
  Current.insert(utf8::byte_index(Current, Caret), Text);

  InputText.set_string(Current);
  Caret += Length;
}

void erase_before_caret(graphics::text &InputText, std::size_t &Caret)
{
  if (Caret == 0)
    return;

  std::string String = InputText.get_string();

  const std::size_t Start = utf8::byte_index(String, Caret - 1);
  const std::size_t End   = utf8::byte_index(String, Caret);

  String.erase(Start, End - Start);
  InputText.set_string(String);

  --Caret;
}

void erase_after_caret(graphics::text &InputText, std::size_t Caret)
{
  std::string String = InputText.get_string();

  if (Caret >= utf8::length(String))
    return;

  const std::size_t Start = utf8::byte_index(String, Caret);
  const std::size_t End   = utf8::byte_index(String, Caret + 1);

  String.erase(Start, End - Start);
  InputText.set_string(String);
}

void submit_input(std::vector<task> &Tasks, graphics::font &Font,
                  graphics::text &InputText, std::size_t &Caret,
                  float &ScrollOffset)
{
  const std::string Title = trim(InputText.get_string());

  InputText.set_string("");
  Caret = 0;

  if (Title.empty())
    return;

  Tasks.push_back({ graphics::text(Font, Title, { 0.f, 0.f }, Font.get_size(),
                                   TextColor),
                    false });
  ScrollOffset = std::numeric_limits<float>::max();
}

float get_row_height(task &Task, float MaxLabelWidth)
{
  if (Task.Label.get_max_width() != MaxLabelWidth)
    Task.Label.set_max_width(MaxLabelWidth);

  const float LineHeight = Task.Label.get_font()->get_line_height();

  return std::max(RowHeight, Task.Label.get_size().y + RowHeight - LineHeight);
}

float layout_rows(std::vector<task> &Tasks, std::vector<row_box> &Boxes,
                  float MaxLabelWidth)
{
  Boxes.clear();

  float Top = 0.f;

  for (task &Task : Tasks)
  {
    const float Height = get_row_height(Task, MaxLabelWidth);

    Boxes.push_back({ Top, Height });
    Top += Height + RowGap;
  }

  return Top;
}

graphics::cursors get_hover_cursor(marx::fVector2 Mouse,
                                   marx::fVector2 WindowSize,
                                   const std::vector<row_box> &Boxes,
                                   float ScrollOffset)
{
  const float          RowWidth = get_row_width(WindowSize);
  const marx::fVector2 InputPosition(Margin, HeaderHeight);

  if (contains(get_icon_position(InputPosition, RowWidth), IconBox, Mouse))
    return graphics::cursors::Hand;

  if (contains(InputPosition, marx::fVector2(RowWidth - IconReserve, RowHeight),
               Mouse))
    return graphics::cursors::Text;

  if (contains(get_clear_position(WindowSize), ClearSize, Mouse))
    return graphics::cursors::Hand;

  if (!in_list(Mouse, WindowSize))
    return graphics::cursors::Arrow;

  for (const row_box &Box : Boxes)
    if (contains(get_row_position(Box, ScrollOffset),
                 marx::fVector2(RowWidth, Box.Height), Mouse))
      return graphics::cursors::Hand;

  return graphics::cursors::Arrow;
}

void draw_rectangle(const graphics::window &Window, marx::fVector2 Position,
                    marx::fVector2 Size, graphics::color Color)
{
  Window.draw(graphics::rectangle(Position, Size, Color));
}

void draw_circle(const graphics::window &Window, marx::fVector2 Center,
                 float Radius, graphics::color Color)
{
  Window.draw(graphics::circle(Center - marx::fVector2(Radius, Radius), Radius,
                               Color));
}

void draw_line(const graphics::window &Window, marx::fVector2 Start,
               marx::fVector2 End, float Thickness, graphics::color Color)
{
  Window.draw(graphics::line({ { Start, End } }, Thickness, Color));
}

void draw_checkbox(const graphics::window &Window, marx::fVector2 Center,
                   bool Done, graphics::color Fill)
{
  if (Done)
  {
    draw_circle(Window, Center, CheckRadius, Accent);
    draw_line(Window, Center + marx::fVector2(-6.f, 0.f),
              Center + marx::fVector2(-2.f, 5.f), 3.f, Background);
    draw_line(Window, Center + marx::fVector2(-2.f, 5.f),
              Center + marx::fVector2(7.f, -5.f), 3.f, Background);
    return;
  }

  draw_circle(Window, Center, CheckRadius, DimColor);
  draw_circle(Window, Center, CheckRadius - 2.f, Fill);
}

void draw_cross(const graphics::window &Window, marx::fVector2 Center,
                graphics::color Color)
{
  draw_line(Window, Center + marx::fVector2(-7.f, -7.f),
            Center + marx::fVector2(7.f, 7.f), 3.f, Color);
  draw_line(Window, Center + marx::fVector2(7.f, -7.f),
            Center + marx::fVector2(-7.f, 7.f), 3.f, Color);
}

void draw_plus(const graphics::window &Window, marx::fVector2 Center,
               graphics::color Color)
{
  draw_line(Window, Center + marx::fVector2(-8.f, 0.f),
            Center + marx::fVector2(8.f, 0.f), 3.f, Color);
  draw_line(Window, Center + marx::fVector2(0.f, -8.f),
            Center + marx::fVector2(0.f, 8.f), 3.f, Color);
}

void draw_strike(const graphics::window &Window, const graphics::text &Label)
{
  const float       LineHeight = Label.get_font()->get_line_height();
  const std::size_t Length     = Label.get_length();

  std::size_t Start = 0;

  while (Start <= Length)
  {
    const marx::fVector2 First = Label.get_caret_position(Start);

    std::size_t End = Start;

    while (End < Length && Label.get_caret_position(End + 1).y == First.y)
      ++End;

    const marx::fVector2 Last = Label.get_caret_position(End);
    const float          Y    = First.y + LineHeight / 2.f;

    draw_line(Window, { First.x, Y }, { Last.x, Y }, 2.f, DimColor);

    Start = End + 1;
  }
}

void draw_task(const graphics::window &Window, task &Task,
               marx::fVector2 Position, float RowWidth, float Height,
               marx::fVector2 Mouse, bool MouseInList)
{
  const marx::fVector2 RowSize(RowWidth, Height);

  const bool Hovered = MouseInList && contains(Position, RowSize, Mouse);

  const graphics::color Fill = Hovered ? HoverColor : RowColor;

  draw_rectangle(Window, Position, RowSize, Fill);

  const marx::fVector2 CheckCenter(Position.x + Padding + CheckRadius,
                                   Position.y + RowHeight / 2.f);

  draw_checkbox(Window, CheckCenter, Task.Done, Fill);

  const float LineHeight = Task.Label.get_font()->get_line_height();

  place(Task.Label, { Position.x + LabelOffset,
                      Position.y + (RowHeight - LineHeight) / 2.f });
  Window.draw(Task.Label);

  if (Task.Done)
    draw_strike(Window, Task.Label);

  const marx::fVector2 IconPosition = get_icon_position(Position, RowWidth);
  const bool           IconHovered =
      MouseInList && contains(IconPosition, IconBox, Mouse);

  draw_cross(Window, IconPosition + IconBox / 2.f,
             IconHovered ? Danger : DimColor);
}
} // namespace

int main()
{
  graphics::window Window(marx::iVector2(800, 600), "To do");
  Window.enable_vsync(true);
  Window.set_min_size({ 420, 320 });

  graphics::font Font(FontPath, 30);

  if (!Font.is_loaded())
    return 1;

  graphics::font TitleFont(Font, 64);
  graphics::font SmallFont(Font, 22);

  graphics::clock Clock;

  graphics::text Title(TitleFont, "To do", { Margin, 16.f },
                       TitleFont.get_size(), TextColor);
  graphics::text InputText(Font, "", { 0.f, 0.f }, Font.get_size(), TextColor);
  graphics::text Placeholder(Font, "What needs doing?", { 0.f, 0.f },
                             Font.get_size(), DimColor);
  graphics::text EmptyLabel(SmallFont, "Nothing to do. Add a task above.",
                            { 0.f, 0.f }, SmallFont.get_size(), DimColor);
  graphics::text Stats(SmallFont, "", { 0.f, 0.f }, SmallFont.get_size(),
                       DimColor);
  graphics::text ClearLabel(SmallFont, "Clear done", { 0.f, 0.f },
                            SmallFont.get_size(), TextColor);

  std::vector<task>    Tasks;
  std::vector<row_box> Boxes;
  std::size_t          Caret        = 0;
  float                ScrollOffset = 0.f;
  float                InputScroll  = 0.f;
  float                LastTyped    = 0.f;
  std::string          WindowName   = "To do";

  while (Window.is_open())
  {
    Clock.update();
    const float Time = Clock.get_time_passed();

    marx::fVector2 Size = get_window_size(Window);

    layout_rows(Tasks, Boxes, get_max_label_width(Size));

    while (auto Event = Window.poll())
    {
      if (Event->is<events::Close>())
        Window.close();

      else if (auto Press = Event->get_if<events::ButtonDown>())
      {
        if (Press->Button == mouse::buttons::Left)
        {
          const marx::fVector2 Point(static_cast<float>(Press->x),
                                     static_cast<float>(Press->y));

          const float          RowWidth = get_row_width(Size);
          const marx::fVector2 InputPosition(Margin, HeaderHeight);

          LastTyped = Time;

          if (contains(get_icon_position(InputPosition, RowWidth), IconBox,
                       Point))
            submit_input(Tasks, Font, InputText, Caret, ScrollOffset);

          else if (contains(InputPosition,
                            marx::fVector2(RowWidth - IconReserve, RowHeight),
                            Point))
            Caret = InputText.get_caret_index(Point);

          else if (contains(get_clear_position(Size), ClearSize, Point))
            std::erase_if(Tasks, [](const task &Task) { return Task.Done; });

          else if (in_list(Point, Size))
          {
            for (std::size_t Index = 0; Index < Tasks.size(); ++Index)
            {
              const marx::fVector2 RowPosition =
                  get_row_position(Boxes[Index], ScrollOffset);

              if (!contains(RowPosition,
                            marx::fVector2(RowWidth, Boxes[Index].Height),
                            Point))
                continue;

              if (contains(get_icon_position(RowPosition, RowWidth), IconBox,
                           Point))
                Tasks.erase(Tasks.begin() + Index);
              else
                toggle_task(Tasks[Index]);

              break;
            }
          }
        }
      }

      else if (auto Scroll = Event->get_if<events::Scrolled>())
        ScrollOffset -= Scroll->DeltaY * ScrollStep;

      else if (auto Typed = Event->get_if<events::TextInput>())
      {
        LastTyped = Time;
        insert_text(InputText, Caret, Typed->Text);
      }

      else if (auto Key = Event->get_if<events::KeyDown>())
      {
        LastTyped = Time;

        switch (Key->Key)
        {
          case keyboard::keys::Enter:
          case keyboard::keys::KeypadEnter:
            submit_input(Tasks, Font, InputText, Caret, ScrollOffset);
            break;

          case keyboard::keys::Backspace:
            erase_before_caret(InputText, Caret);
            break;

          case keyboard::keys::Delete:
            erase_after_caret(InputText, Caret);
            break;

          case keyboard::keys::Left:
            if (Caret > 0)
              --Caret;
            break;

          case keyboard::keys::Right:
            if (Caret < InputText.get_length())
              ++Caret;
            break;

          case keyboard::keys::Home:
            Caret = 0;
            break;

          case keyboard::keys::End:
            Caret = InputText.get_length();
            break;

          case keyboard::keys::Escape:
            InputText.set_string("");
            Caret = 0;
            break;

          case keyboard::keys::F11:
            if (!Key->Repeat)
              Window.set_fullscreen(!Window.is_fullscreen());
            break;

          default:
            break;
        }
      }

      layout_rows(Tasks, Boxes, get_max_label_width(Size));
    }

    if (!Window.is_open())
      break;

    Size = get_window_size(Window);

    const float ContentHeight =
        layout_rows(Tasks, Boxes, get_max_label_width(Size));

    const float RowWidth   = get_row_width(Size);
    const float ListBottom = get_list_bottom(Size);
    const float ViewHeight = std::max(0.f, ListBottom - ListTop);
    const float MaxScroll  = std::max(0.f, ContentHeight - ViewHeight);

    ScrollOffset = std::clamp(ScrollOffset, 0.f, MaxScroll);

    const marx::iVector2 MousePixels = Window.get_mouse_position();
    const marx::fVector2 Mouse(static_cast<float>(MousePixels.x),
                               static_cast<float>(MousePixels.y));
    const bool MouseInList = in_list(Mouse, Size);

    Window.set_cursor(get_hover_cursor(Mouse, Size, Boxes, ScrollOffset));

    int DoneCount = 0;
    for (const task &Task : Tasks)
      if (Task.Done)
        ++DoneCount;

    const std::string Name =
        Tasks.empty() ? "To do"
                      : std::format("To do ({}/{})", DoneCount, Tasks.size());

    if (Name != WindowName)
    {
      WindowName = Name;
      Window.set_name(Name);
    }

    Window.clear(Background);

    Window.set_clip({ 0.f, ListTop }, { Size.x, ViewHeight });

    for (std::size_t Index = 0; Index < Tasks.size(); ++Index)
    {
      const marx::fVector2 Position =
          get_row_position(Boxes[Index], ScrollOffset);

      if (Position.y + Boxes[Index].Height < ListTop || Position.y > ListBottom)
        continue;

      draw_task(Window, Tasks[Index], Position, RowWidth, Boxes[Index].Height,
                Mouse, MouseInList);
    }

    if (Tasks.empty())
    {
      place(EmptyLabel,
            { (Size.x - EmptyLabel.get_size().x) / 2.f, ListTop + 32.f });
      Window.draw(EmptyLabel);
    }

    Window.reset_clip();

    Window.draw(Title);

    const marx::fVector2 RowSize(RowWidth, RowHeight);
    const marx::fVector2 InputPosition(Margin, HeaderHeight);

    draw_rectangle(Window, InputPosition - marx::fVector2(2.f, 2.f),
                   RowSize + marx::fVector2(4.f, 4.f), Accent);
    draw_rectangle(Window, InputPosition, RowSize, RowColor);

    draw_checkbox(
        Window,
        InputPosition + marx::fVector2(Padding + CheckRadius, RowHeight / 2.f),
        false, RowColor);

    const marx::fVector2 InputLabelPosition(
        InputPosition.x + LabelOffset,
        InputPosition.y + (RowHeight - Font.get_line_height()) / 2.f);

    const float TextWidth = get_max_label_width(Size);

    const float TextEnd =
        InputText.get_caret_position(InputText.get_length()).x -
        InputText.get_position().x;
    const float CaretOffset =
        InputText.get_caret_position(Caret).x - InputText.get_position().x;

    InputScroll =
        std::clamp(InputScroll, 0.f, std::max(0.f, TextEnd - TextWidth + 2.f));

    if (CaretOffset - InputScroll > TextWidth - 2.f)
      InputScroll = CaretOffset - TextWidth + 2.f;

    if (CaretOffset - InputScroll < 0.f)
      InputScroll = CaretOffset;

    place(InputText,
          { InputLabelPosition.x - InputScroll, InputLabelPosition.y });
    place(Placeholder, InputLabelPosition);

    Window.set_clip({ InputLabelPosition.x, InputPosition.y },
                    { TextWidth, RowHeight });

    if (InputText.get_string().empty())
      Window.draw(Placeholder);
    else
      Window.draw(InputText);

    if (Window.is_focused() && std::fmod(Time - LastTyped, 1.f) < 0.5f)
    {
      const marx::fVector2 CaretPosition = InputText.get_caret_position(Caret);
      const float          CursorHeight  = Font.get_line_height() * 0.8f;

      draw_rectangle(
          Window,
          { CaretPosition.x,
            CaretPosition.y + (Font.get_line_height() - CursorHeight) / 2.f },
          { 2.f, CursorHeight }, TextColor);
    }

    Window.reset_clip();

    const marx::fVector2 PlusPosition =
        get_icon_position(InputPosition, RowWidth);
    const marx::fVector2 PlusCenter = PlusPosition + IconBox / 2.f;

    draw_circle(Window, PlusCenter, IconSize / 2.f,
                contains(PlusPosition, IconBox, Mouse) ? AccentHover : Accent);
    draw_plus(Window, PlusCenter, Background);

    if (MaxScroll > 0.f)
    {
      const float ThumbHeight =
          std::max(24.f, ViewHeight * ViewHeight / ContentHeight);
      const float ThumbY =
          ListTop + ScrollOffset / MaxScroll * (ViewHeight - ThumbHeight);

      draw_rectangle(Window, { Size.x - 8.f, ThumbY }, { 4.f, ThumbHeight },
                     DimColor);
    }

    draw_rectangle(Window, { Margin, ListBottom }, { RowWidth, 1.f }, RowColor);

    relabel(Stats, std::format("{} of {} done", DoneCount, Tasks.size()));
    place(Stats,
          { Margin,
            ListBottom + (FooterHeight - SmallFont.get_line_height()) / 2.f });
    Window.draw(Stats);

    const marx::fVector2 ClearPosition = get_clear_position(Size);

    draw_rectangle(Window, ClearPosition, ClearSize,
                   contains(ClearPosition, ClearSize, Mouse) ? HoverColor
                                                              : RowColor);

    place(ClearLabel,
          { ClearPosition.x + (ClearSize.x - ClearLabel.get_size().x) / 2.f,
            ClearPosition.y +
                (ClearSize.y - SmallFont.get_line_height()) / 2.f });
    Window.draw(ClearLabel);

    Window.swap();
  }

  return 0;
}

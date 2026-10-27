#pragma once

namespace marx::graphics::keyboard
{
// clang-format off
enum class keys
{
  A, B, C, D, E, 
  F, G, H, I, J, 
  K, L, M, N, O, 
  P, Q, R, S, T,
  U, V, W, X, Y,
  Z,

  Zero, One, Two, Three, Four,
  Five, Six, Seven, Eight, Nine,

  F1, F2, F3, F4, F5,
  F6, F7, F8, F9, F10,
  F11, F12, F13, F14, F15,
  F16, F17, F18, F19,F20,
  F21, F22, F23, F24,

  LeftShift, RightShift, 
  LeftControl, RightControl,
  LeftAlt, RightAlt,
  LeftSuper, RightSuper,
  CapsLock, NumLock, ScrollLock,

  Up, Down, Left, Right,
  Home, End,
  PageUp, PageDown, Insert, Delete,

  Escape,
  Tab,
  Enter,
  Backspace,
  Space,
  PrintScreen,
  Pause,
  Menu,

  Grave,
  Minus,
  Equal,
  LeftBracket,
  RightBracket,
  Backslash,
  Semicolon,
  Apostrophe,
  Comma,
  Period,
  Slash,

  KeypadZero, KeypadOne, KeypadTwo, KeypadThree, KeypadFour,
  KeypadFive, KeypadSix, KeypadSeven, KeypadEight, KeypadNine,
  KeypadAdd, KeypadSubtract, KeypadMultiply, KeypadDivide,
  KeypadDecimal, KeypadEnter,

  VolumeUp, VolumeDown, VolumeMute, 
  MediaPlay, MediaStop, MediaPrevious, MediaNext,

  Null
};
// clang-format on

char key_to_ascii(keys Key);
bool key_down(keys Key);

} // namespace marx::graphics::keyboard

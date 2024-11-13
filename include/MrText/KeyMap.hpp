#pragma once

enum key {
  null = 0x00,
  A = 0x01,
  B = 0x02,
  C = 0x03,
  D = 0x04,
  E = 0x05,
  F = 0x06,
  G = 0x07,
  H = 0x08,
  I = 0x09,
  J = 0x0A,
  K = 0x0B,
  L = 0x0C,
  M = 0x0D,
  N = 0x0E,
  O = 0x0F,
  P = 0x10,
  Q = 0x11,
  R = 0x12,
  S = 0x13,
  T = 0x14,
  U = 0x15,
  V = 0x16,
  W = 0x17,
  X = 0x18,
  Y = 0x19,
  Z = 0x1A,
  Esc = '\x1b',
  L_Brac = '[',
  Left = 'D',
  Right = 'C',
  Up = 'A',
  Down = 'B',
  Home = 'H',
  End = 'F',
  TabBack = 'Z'
};

void kill_line() noexcept;
void cursor_down() noexcept;
void cursor_up() noexcept;
void cursor_left() noexcept;
void cursor_right() noexcept;
void deleteChar() noexcept;
void backspace() noexcept;

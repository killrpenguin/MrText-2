#pragma once
#include <unordered_map>
#include <cstdint>

class KeyMap {};

class KeyMapData {
  using term_size = std::uint32_t;

public:
  term_size cur_row;
  term_size cur_col;
  term_size last_row; // # of lines in document.
  unsigned int cur_line_len;

};

class EnumHash {
  template <typename Key>
  typename std::enable_if<std::is_enum<Key>::value, std::size_t>::type
  operator()(Key const value) const {
    return static_cast<std::size_t>(value);
  }
};

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
  Escape = '\x1b',
  L_Brac = '[',
  Left = 'D',
  Right = 'C',
  Up = 'A',
  Down = 'B',
  TabBack = 'Z',

  Home = '1', // 'H' || '7'
  Delete = '3',
  End = '4', // '8' || 'F'
  Page_Up = '5',
  Page_Down = '6',

};

void kill_line(const KeyMapData &data) noexcept;
void cursor_down(const KeyMapData &data) noexcept;
void cursor_up(const KeyMapData &data) noexcept;
void cursor_left(const KeyMapData &data) noexcept;
void cursor_right(const KeyMapData &data) noexcept;
void delete_char(const KeyMapData &data) noexcept;
void backspace(const KeyMapData &data) noexcept;
void home(const KeyMapData &data) noexcept;
void end(const KeyMapData &data) noexcept;
void page_up(const KeyMapData &data) noexcept;
void page_down(const KeyMapData &data) noexcept;
void escape(const KeyMapData &data) noexcept;
void line_beginning(const KeyMapData &data) noexcept;

#pragma once

#include "MrText/KeyMap.hpp"
#include <fstream>

enum EventType {
  Keyboard = 1,
  Mouse = 2,
  CursorMove = 3,
};

class Event {
  using buffer_type = std::array<char, 4>;
  using cursor_pos = const std::tuple<unsigned int, unsigned int>;
  typedef void (*functionPtr)(const KeyMapData &data);

  bool run;
  EventType event_type;
  char output;

public:
  auto loop(const KeyMapData &data, buffer_type *buffer) noexcept -> void;

  auto parse_event() noexcept -> bool;

  auto parse_key() noexcept -> functionPtr;
  auto parse_mouse() noexcept -> void;
  auto parse_pos(std::streambuf *pbuf) noexcept -> cursor_pos;
};

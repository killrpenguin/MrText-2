#include "MrText/KeyMap.hpp"
#include "MrText/MrText.hpp"

#include <cstdio>
#include <iostream>

void kill_line(const KeyMapData &data) noexcept {
  if (data.cur_line_len != 0) {
    std::cout << escSeqs::CLRLN_CURSRFRD;
    std::cout.flush();
  }
}
void cursor_down(const KeyMapData &data) noexcept {
  if (data.last_row != data.cur_row) {
    std::cout << escSeqs::MVDOWN;
    std::cout.flush();
  }
}
void cursor_up(const KeyMapData &data) noexcept {
  if (data.cur_row != 1) {
    std::cout << escSeqs::MVUP;
    std::cout.flush();
  }
}
void cursor_left(const KeyMapData &data) noexcept {
  if (data.cur_col != 1) {
    std::cout << escSeqs::MVLEFT;
    std::cout.flush();
  }
}
void cursor_right(const KeyMapData &data) noexcept {
  if (data.cur_col < data.cur_line_len) {
    std::cout << escSeqs::MVRIGHT;
    std::cout.flush();
  }
}
void backspace(const KeyMapData &data) noexcept {
  if (data.cur_col != 1) {
    std::cout << escSeqs::MVLEFT << escSeqs::CLRLN_CURSRFRD;
    std::cout.flush();
  }
}
void delete_char(const KeyMapData &data) noexcept {
  if (data.cur_line_len != 0) {
    std::cout << "Delete Key";
    std::cout.flush();
  }
}
void home(const KeyMapData &data) noexcept {
  // placeholder logic.
  if (data.cur_row > 0) {
    std::cout << "Home Key.";
    std::cout.flush();
  }
}
void end(const KeyMapData &data) noexcept {
  if (data.cur_col != data.cur_line_len) {
    std::cout << "End Key.";
    std::cout.flush();
  }
}
void page_up(const KeyMapData &data) noexcept {
  // placeholder logic.
  if (data.cur_row > 0) {
    std::cout << "Page Up Key.";
    std::cout.flush();
  }
}
void page_down(const KeyMapData &data) noexcept {
  // placeholder logic.
  if (data.cur_row > 0) {
    std::cout << "Page Down Key. ";
    std::cout.flush();
  }
}
void escape(const KeyMapData &data) noexcept {
  // placeholder logic.
  if (data.cur_row > 0) {
    std::cout << "Escape Key. ";
    std::cout.flush();
  }
}
void line_beginning(const KeyMapData &data) noexcept {
  std::printf("%s\x1b[%d;%dH%s", escSeqs::HIDECURSOR.c_str(), data.cur_row, 1,
              escSeqs::SHOWCURSOR.c_str());
  std::cout.flush();
}

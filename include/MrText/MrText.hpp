#pragma once

#include "KeyMap.hpp"

#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <termios.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

class MrText {
  typedef void (*functionPtr)();
  using cursor_pos = const std::tuple<unsigned int, unsigned int>;
  using unordered_map = std::unordered_map<key, functionPtr>;
  using utf8_array = std::array<char, 4>;

private:
  bool running;
  termios original_term;
  utf8_array utf8_buf;

  std::fstream file_strm;
  const std::filesystem::path file_name;

  void raw_mode();

  void init_file() noexcept;
  void display_file() noexcept;
  void create_ctrl_key_map() noexcept;
  void position_cursor(unsigned int row, unsigned int col) noexcept;

protected:
  unordered_map key_map;
  std::uint32_t row;
  std::uint32_t col;

public:
  MrText() noexcept;
  MrText(std::string file_name) noexcept;
  ~MrText();

  void set_buffer_val(utf8_array test_buf) noexcept;

  cursor_pos parse_pos(std::streambuf *pbuf) noexcept;

  key parse_key() noexcept;
  void parse_ascii_key(const char buffer) noexcept;

  void parse_ctrl_event() noexcept;
  void screen_size() noexcept;
  void run() noexcept;
};


class hex {
public:
  const static int ctrlBegin;
  const static int ctrlEnd;
  const static int exitEditor;
  const static int asciiBegin;
  const static int asciiEnd;
};

class escSeqs {
public:
  const static std::string RN;
  const static std::string CLR_SCRN;
  const static std::string CURSOR_HOME;
  const static std::string CLR_LN;
  const static std::string CLRSCRN_CURSREND;
  const static std::string CLRLN_CURSRFRD;
  const static std::string CLRLN_CURSRBK;
  const static std::string GETCURSORPOS;
  const static std::string SHOWCURSOR;
  const static std::string HIDECURSOR;
  const static std::string SCROLLUP;
  const static std::string SCROLLDOWN;
  const static std::string MVLEFT;
  const static std::string MVRIGHT;
  const static std::string MVUP;
  const static std::string MVDOWN;
  const static std::string MVFARRIGHT;
  const static std::string MVFARDOWN;
};

#pragma once

#include "ArrayBuffer.hpp"
#include "MrText/KeyMap.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
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

using cursor_pos = const std::tuple<unsigned int, unsigned int>;
extern bool RESIZE_SWITCH;
class KeyMapData;

class MrText {
  typedef void (*functionPtr)(const KeyMapData &data);
  using unordered_map = std::unordered_map<key, functionPtr>;

private:
  bool running;
  bool line_buffering;
  termios original_term;
  ArrayBuffer<char> buffer;
  KeyMapData app_data;

  std::fstream file_strm;
  const std::filesystem::path file_name;

  auto raw_mode() -> void;
  auto screen_size() noexcept -> void;
  auto toggle_linebuffering() noexcept -> void;

  auto init_file() noexcept -> void;
  auto display_file() noexcept -> void;
  auto create_ctrl_key_map() noexcept -> void;
  static auto position_cursor(unsigned int row, unsigned int col) noexcept
      -> void;
  auto display_key() noexcept -> void;

protected:
  unordered_map key_map;

public:
  MrText();
  MrText(const std::string &file_name);
  ~MrText();

  auto parse_key(const char *zero) noexcept -> void;
  key parse_escape_keys(const char *two) noexcept;
  static auto parse_pos(std::streambuf *pbuf) noexcept -> cursor_pos;
  auto parse_ascii_key(const char buffer) noexcept -> void;
  auto update(unsigned int line_len) noexcept -> void;
  auto run() noexcept -> void;
};

class ResizeStatus {
public:
  ResizeStatus() noexcept;
  bool resize;
};

class hex {
public:
  const static int ctrlBegin;
  const static int ctrlEnd;
  const static int exitEditor;
  const static int asciiBegin;
  const static int asciiEnd;
  const static int l_brac;
};

class escSeqs {
public:
  const static std::string RNL;
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

#include "MrText/ArrayBuffer.hpp"
#include "MrText/KeyMap.hpp"
#include "MrText/RopeConstants.hpp"
#include <MrText/MrText.hpp>

#include <asm-generic/ioctls.h>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <unistd.h>
#include <vector>

#include <sys/ioctl.h>
#include <termios.h>

bool RESIZE_SWITCH = false;

static void handle_resize(int sig) {
  if (sig != 0) {
  }
  RESIZE_SWITCH = true;
}

MrText::MrText()
    : running{true}, line_buffering{true}, original_term{}, app_data{
                                                                KeyMapData()} {

  raw_mode();
  std::cout << escSeqs::CLR_SCRN << '\n';
}

MrText::MrText(const std::string &file_name)
    : running{true}, line_buffering{true},
      original_term{}, app_data{KeyMapData()}, file_name{file_name} {

  raw_mode();
  std::cout << escSeqs::CLR_SCRN << '\n';
}

MrText::~MrText() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_term);
  file_strm.close();
  toggle_linebuffering();
  std::cout << escSeqs::CLR_SCRN << escSeqs::CURSOR_HOME << '\n';
}

void MrText::position_cursor(unsigned int row_pos,
                             unsigned int col_pos) noexcept {

  std::printf("%s\x1b[%d;%dH%s", escSeqs::HIDECURSOR.c_str(), row_pos, col_pos,
              escSeqs::SHOWCURSOR.c_str());
  std::cout.flush();
}

void MrText::toggle_linebuffering() noexcept {
  if (line_buffering) {
    line_buffering = false;
    // Disable stdout line buffering.
    setvbuf(stdout, nullptr, _IONBF, BUFSIZ);
  } else {
    line_buffering = true;
    // Enable stdout line buffering.
    setvbuf(stdout, nullptr, _IOLBF, BUFSIZ);
  }
}

auto MrText::parse_pos(std::streambuf *pbuf) noexcept -> cursor_pos {
  unsigned int row{};
  unsigned int result{};

  std::vector<unsigned int> number_buffer;

  while (true) {
    char const letter = static_cast<char>(pbuf->sbumpc());
    if (letter == 'R' || letter == -1) {
      break;
    }
    if (letter >= '0' && letter <= '9') {
      number_buffer.push_back(letter - '0');
    }
    if (letter == ';') {
      for (unsigned int const num : number_buffer) {
        result = result * RopeConst::Ten + num;
      }
      row = result;
      number_buffer.clear();
      result = 0;
    }
  }
  for (unsigned int const num : number_buffer) {
    result = result * RopeConst::Ten + num;
  }
  assert((row != 0) && "Could not get row size.");
  assert((result != 0) && "Could not get column size.");
  return std::tuple<unsigned int, unsigned int>{row, result};
}

auto MrText::display_file() noexcept -> void {
  position_cursor(1, 1);
  std::string line{};
  while (std::getline(file_strm, line, '\n')) {
    if (line.length() <= app_data.cur_col) {
      std::cout << line << escSeqs::RNL;
    } else {
      size_t const space_pos = line.find_last_of(' ', app_data.cur_col);
      std::cout << line.substr(0, space_pos) << escSeqs::RNL
                << line.substr(space_pos, line.length()) << escSeqs::RNL;
    }
    line.clear();
  }
}

auto MrText::screen_size() noexcept -> void {
  struct winsize win_size {};
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size) == 0 ||
      win_size.ws_col != 0) {
    app_data.cur_row = win_size.ws_row;
    app_data.cur_col = win_size.ws_col;
  } else {
    std::cout << escSeqs::MVFARDOWN << escSeqs::MVFARRIGHT
              << escSeqs::GETCURSORPOS;

    cursor_pos size{parse_pos(std::cin.rdbuf())};

    app_data.cur_row = std::get<0>(size);
    app_data.cur_col = std::get<1>(size);

    assert((app_data.cur_row != 0) && "Could not get row size.");
    assert((app_data.cur_col != 0) && "Could not get column size.");
  }
}

auto MrText::raw_mode() -> void {
  if (tcgetattr(STDIN_FILENO, &original_term) != 0) {
    throw std::runtime_error("Unable to backup terminal settings.");
    std::abort();
  }
  struct termios raw_term {};
  raw_term = original_term;

  raw_term.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw_term.c_oflag &= ~(OPOST);
  raw_term.c_cflag |= (CS8);
  raw_term.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw_term.c_cc[VMIN] = 0;
  raw_term.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw_term) != 0) {
    throw std::runtime_error("Unable to set raw terminal settings.");
  }
}

auto MrText::init_file() noexcept -> void {
  if (file_name.empty() && !file_strm.is_open()) {
    file_strm.open("new.txt");
  } else {
    file_strm.open(file_name, std::ios_base::in | std::ios_base::out);
  }
}

auto MrText::display_key() noexcept -> void {
  for (const char letter : buffer) {
    if (letter != 0) {
      std::cout << letter;
    }
  }
  std::cout.flush();
}

void MrText::parse_ascii_key(const char buf) noexcept {
  if (buf >= hex::ctrlBegin && buf <= hex::ctrlEnd) {
    key const key_press{static_cast<key>(buf)};
    if (buf == hex::exitEditor) {
      std::cout << escSeqs::CLR_SCRN << '\n';
      running = false;
    }
    if (key_map.contains(key_press)) {
      functionPtr callCtrlKeyFn = key_map[key_press];
      callCtrlKeyFn(app_data);
    }
  }
  if (buf >= hex::asciiBegin && buf <= hex::asciiEnd) {
    std::cout << buf;
    std::cout.flush();
  }
}

void MrText::parse_key(const char *zero) noexcept {
  key key_press = key::null;
  if (*zero == '\x1b') {
    char *one = buffer.next();
    char *two = buffer.next();
    if (*one == hex::l_brac) {
      key_press = parse_escape_keys(two);
    } else {
      switch (*two) {
      case 'H':
        key_press = key::Home;
        break;
      case 'F':
        key_press = key::End;
        break;
      default:
        break;
      }
    }
  }
  if (key_press != key::null) {
    if (key_map.contains(key_press)) {
      functionPtr callCtrlKeyFn = key_map[key_press];
      callCtrlKeyFn(app_data);
    }
  }
}

auto MrText::parse_escape_keys(const char *two) noexcept -> key {
  char *three = buffer.next();
  if (*two >= '0' && *two <= '9' && *three == '~') {
    switch (*two) {
    case '1':
      return key::Home;
    case '3':
      return key::Delete;
    case '4':
      return key::End;
    case '5':
      return key::Page_Up;
    case '6':
      return key::Page_Down;
    case '7':
      return key::Home;
    case '8':
      return key::End;
    default:
      break;
    }
  } else {
    switch (*two) {
    case 'A':
      return key::Up;
    case 'B':
      return key::Down;
    case 'C':
      return key::Right;
    case 'D':
      return key::Left;
    case 'H':
      return key::Home;
    case 'F':
      return key::End;
    default:
      break;
    }
  }
  return key::null;
}

auto MrText::update(unsigned int line_len) noexcept -> void {
  buffer.reset();

  if (RESIZE_SWITCH) {
    RESIZE_SWITCH = false;
    screen_size();
  }
  std::cout << escSeqs::GETCURSORPOS;
  std::cout.flush();
  cursor_pos pos = parse_pos(std::cin.rdbuf());

  app_data.cur_row = std::get<0>(pos);
  app_data.cur_col = std::get<1>(pos);
  app_data.cur_line_len = line_len;
  app_data.last_row = RopeConst::TempLastRow; // temp value.

  assert((app_data.cur_row != 0) && "Could not get row size.");
  assert((app_data.cur_col != 0) && "Could not get column size.");
}

auto MrText::run() noexcept -> void {
  init_file();
  screen_size();

  if (file_name != "new.txt") {
    display_file();
  }

  signal(SIGWINCH, handle_resize);
  position_cursor(1, 1);
  create_ctrl_key_map();
  buffer_type *arr_buf = buffer.pInner();

  if (arr_buf == nullptr) {
    std::abort();
  }

  while (running) {
    update(RopeConst::TempLineLen); // temp value;
    if (read(STDIN_FILENO, arr_buf, 4) == 1) {
      parse_ascii_key(buffer[0]);
    } else {
      if (buffer[1] != 0) {
        parse_key(buffer.next());
      }
    }
  }
}

auto MrText::create_ctrl_key_map() noexcept -> void {
  key_map[key::K] = kill_line;
  key_map[key::N] = cursor_down;
  key_map[key::P] = cursor_up;
  key_map[key::F] = cursor_right;
  key_map[key::B] = cursor_left;
  key_map[key::A] = line_beginning;

  key_map[key::H] = backspace;
  key_map[key::Delete] = delete_char;
  key_map[key::Home] = home;
  key_map[key::End] = end;
  key_map[key::Page_Up] = page_up;
  key_map[key::Page_Down] = page_down;

  key_map[key::Down] = cursor_down;
  key_map[key::Up] = cursor_up;
  key_map[key::Right] = cursor_right;
  key_map[key::Left] = cursor_left;
}

class enum_hash {
  template <typename Key>
  typename std::enable_if<std::is_enum<Key>::value, std::size_t>::type
  operator()(Key const value) const {
    return static_cast<std::size_t>(value);
  }
};

const int hex::exitEditor{0x11};
const int hex::ctrlBegin{0x01};
const int hex::ctrlEnd{0x1F};
const int hex::asciiBegin{0x20};
const int hex::asciiEnd{0x7E};
const int hex::l_brac{0x5B};

const std::string escSeqs::RNL{"\r\n"};
const std::string escSeqs::CURSOR_HOME{"\x1b[H"};
const std::string escSeqs::CLR_SCRN{"\x1b[2J"};
const std::string escSeqs::CLR_LN{"\x1b[2K"};
const std::string escSeqs::CLRSCRN_CURSREND{"\x1b[0J"};
const std::string escSeqs::CLRLN_CURSRFRD{"\x1b[0K"};
const std::string escSeqs::CLRLN_CURSRBK{"\x1b[1K"};
const std::string escSeqs::GETCURSORPOS{"\x1b[6n"};
const std::string escSeqs::SHOWCURSOR{"\x1b[?25h"};
const std::string escSeqs::HIDECURSOR{"\x1b[?25l"};
const std::string escSeqs::SCROLLUP{"\x1b[1T"};
const std::string escSeqs::SCROLLDOWN{"\x1b[1S"};
const std::string escSeqs::MVLEFT{"\x1b[1D"};
const std::string escSeqs::MVRIGHT{"\x1b[1C"};
const std::string escSeqs::MVUP{"\x1b[1A"};
const std::string escSeqs::MVDOWN{"\x1b[1B"};
const std::string escSeqs::MVFARRIGHT{"\x1b[999C"};
const std::string escSeqs::MVFARDOWN{"\x1b[999B"};

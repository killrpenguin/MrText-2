#include "MrText/KeyMap.hpp"
#include <MrText/MrText.hpp>

#include <cassert>
#include <stdexcept>

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <sys/ioctl.h>
#include <termios.h>

MrText::MrText() noexcept : running(true), row(0), col(0) {
  raw_mode();
  std::cout << escSeqs::CLR_SCRN << std::endl;
}

MrText::MrText(std::string file_name) noexcept
    : running(true), file_name(file_name), row(0), col(0) {
  raw_mode();
  std::cout << escSeqs::CLR_SCRN << std::endl;
}

MrText::~MrText() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_term);
  file_strm.close();
  setvbuf(stdout, NULL, _IOLBF, BUFSIZ); // Enable stdout line buffering.
  std::cout << escSeqs::CLR_SCRN << escSeqs::CURSOR_HOME << std::endl;
}

void MrText::set_buffer_val(utf8_array test_buf) noexcept {
  utf8_buf = test_buf;
}

void MrText::position_cursor(unsigned int row_pos,
                             unsigned int col_pos) noexcept {
  std::printf("%s\x1b[%d;%dH%s", escSeqs::HIDECURSOR.c_str(), row_pos, col_pos,
              escSeqs::SHOWCURSOR.c_str());
  std::cout.flush();
}

const std::tuple<unsigned int, unsigned int>
MrText::parse_pos(std::streambuf *pbuf) noexcept {
  unsigned int row_len{};
  unsigned int result{};

  std::vector<unsigned int> number_buffer;
  while (true) {
    char ch = static_cast<char>(pbuf->sbumpc());
    if (ch == 'R' || ch == -1) {
      break;
    }
    if (ch >= '0' && ch <= '9') {
      number_buffer.push_back(ch - '0');
    }
    if (ch == ';') {
      for (unsigned int num : number_buffer) {
        result = result * 10 + num;
      }
      row_len = result;
      number_buffer.clear();
      result = 0;
    }
  }
  for (unsigned int num : number_buffer) {
    result = result * 10 + num;
  }
  return std::tuple<unsigned int, unsigned int>{row_len, result};
}

void MrText::display_file() noexcept {
  position_cursor(1, 1);
  std::string line{};
  while (std::getline(file_strm, line, '\n')) {
    if (line.length() <= col) {
      std::cout << line << escSeqs::RN;
    } else {
      size_t space_pos = line.find_last_of(" ", col);
      std::cout << line.substr(0, space_pos) << escSeqs::RN
                << line.substr(space_pos, line.length()) << escSeqs::RN;
    }
    line.clear();
  }
}

void MrText::screen_size() noexcept {
  struct winsize win_size {};
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_size) == 0 ||
      win_size.ws_col != 0) {
    row = win_size.ws_row;
    col = win_size.ws_col;
  } else {
    std::cout << escSeqs::MVFARDOWN << escSeqs::MVFARRIGHT
              << escSeqs::GETCURSORPOS;

    std::tuple<unsigned int, unsigned int> size{parse_pos(std::cin.rdbuf())};

    row = std::get<0>(size);
    col = std::get<1>(size);
    assert((row != 0) && "Could not get row size.");
    assert((col != 0) && "Could not get column size.");
  }
}

void MrText::raw_mode() {
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

void MrText::init_file() noexcept {
  if (file_name.empty() && !file_strm.is_open()) {
    file_strm.open("new.txt");
  } else {
    file_strm.open(file_name, std::ios_base::in | std::ios_base::out);
  }
}

void MrText::parse_ascii_key(const char buf) noexcept {
  if (buf >= hex::ctrlBegin && buf <= hex::ctrlEnd) {
    key key_press{static_cast<key>(buf)};
    if (buf == hex::exitEditor) {
      std::cout << escSeqs::CLR_SCRN << std::endl;
      running = false;
    }
    if (key_map.contains(key_press)) {
      functionPtr callCtrlKeyFn = key_map[key_press];
      callCtrlKeyFn();
    }
  }
  if (buf >= hex::asciiBegin && buf <= hex::asciiEnd) {
    std::cout << buf;
    std::cout.flush();
  }
}

key MrText::parse_key() noexcept {
  for (const char byte : utf8_buf) {
    if (byte == 0) {
      return key::null;
    }
    if (byte == key::Esc) {
      return key::Esc;
    }
  }
  return key::Esc;
}

void MrText::run() noexcept {
  init_file();
  screen_size();
  if (file_name != "new.txt") {
    display_file();
  }

  position_cursor(1, 1);
  create_ctrl_key_map();

  while (running) {
    if (!utf8_buf.empty()) {
      utf8_buf.fill(0);
    }
    if (read(STDIN_FILENO, &utf8_buf, 4) == 1) {
      parse_ascii_key(utf8_buf[0]);
    } else {
      parse_key();
    }
  }
}

void MrText::create_ctrl_key_map() noexcept {
  key_map[key::K] = kill_line;
  key_map[key::N] = cursor_down;
  key_map[key::P] = cursor_up;
  key_map[key::F] = cursor_right;
  key_map[key::B] = cursor_left;
  key_map[key::H] = backspace;
  key_map[key::Left] = cursor_left;
  key_map[key::Down] = cursor_down;
  key_map[key::Up] = cursor_up;
  key_map[key::Right] = cursor_right;
}

void kill_line() noexcept {
  std::cout << escSeqs::CLRLN_CURSRFRD;
  std::cout.flush();
}
void cursor_down() noexcept {
  std::cout << escSeqs::MVDOWN;
  std::cout.flush();
}
void cursor_up() noexcept {
  std::cout << escSeqs::MVUP;
  std::cout.flush();
}
void cursor_left() noexcept {
  std::cout << escSeqs::MVLEFT;
  std::cout.flush();
}
void cursor_right() noexcept {
  std::cout << escSeqs::MVRIGHT;
  std::cout.flush();
}
void backspace() noexcept {
  std::cout << escSeqs::MVLEFT << escSeqs::CLRLN_CURSRFRD;
  std::cout.flush();
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

const std::string escSeqs::RN{"\r\n"};
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

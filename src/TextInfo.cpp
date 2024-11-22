#include "MrText/TextInfo.hpp"
#include <string_view>

TextInfo::TextInfo() noexcept : bytes{0}, chars{0}, line_breaks{0} {}

TextInfo::TextInfo(Count bytes, Count chars, Count line_breaks) noexcept
    : bytes{bytes}, chars{chars}, line_breaks{line_breaks} {};

TextInfo::TextInfo(std::string_view text) noexcept
    : bytes{text.length()}, chars{text.length()}, line_breaks{0} {

  count_line_breaks(text);
};

auto TextInfo::operator+(const TextInfo &rval) -> TextInfo {
  return TextInfo((bytes + rval.bytes), (chars + rval.chars),
                  (line_breaks + rval.line_breaks));
}

auto TextInfo::operator-(const TextInfo &rval) -> TextInfo {
  Count new_bytes{0};
  Count new_chars{0};
  Count new_line_count{0};

  if (bytes >= rval.bytes) {
    new_bytes = bytes - rval.bytes;
  } else {
    new_bytes = 0;
  }
  if (chars >= rval.chars) {
    new_chars = chars - rval.chars;
  } else {
    new_chars = 0;
  }
  if (line_breaks >= rval.line_breaks) {
    new_line_count = line_breaks - rval.line_breaks;
  } else {
    new_line_count = 0;
  }

  return TextInfo(new_bytes, new_chars, new_line_count);
}

void TextInfo::operator+=(const TextInfo &rval) {
  bytes += rval.bytes;
  chars += rval.chars;
  line_breaks += rval.line_breaks;
}

void TextInfo::operator-=(const TextInfo &rval) {
  if (bytes >= rval.bytes) {
    bytes -= rval.bytes;
  } else {
    bytes = 0;
  }
  if (chars >= rval.chars) {
    chars = chars - rval.chars;
  } else {
    chars = 0;
  }
  if (line_breaks >= rval.line_breaks) {
    line_breaks = line_breaks - rval.line_breaks;
  } else {
    line_breaks = 0;
  }
}

void TextInfo::count_line_breaks(const std::string_view text) noexcept {
  if (bytes == 0 && chars == 0) {
    return;
  }
  for (auto letter : text) {
    if (letter == '\n') {
      line_breaks += 1;
    }
  }
}

auto TextInfo::contains_line_break() const noexcept -> bool {
  return line_breaks != 0;
}

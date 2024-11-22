#pragma once
//#include "MrText/RopeConstants.hpp"

#include <string>
#include <string_view>

class TextInfo {
  using Count = unsigned long;
public:
  Count bytes;
  Count chars;
  Count line_breaks;

  TextInfo() noexcept;
  explicit TextInfo(Count bytes, Count chars, Count line_breaks) noexcept;
  explicit TextInfo(std::string_view text) noexcept;

  void operator+=(const TextInfo &rval);
  void operator-=(const TextInfo &rval);

  auto operator+(const TextInfo &rval) -> TextInfo;
  auto operator-(const TextInfo &rval) -> TextInfo;

  void count_line_breaks(const std::string_view text) noexcept;
  bool contains_line_break() const noexcept;
};

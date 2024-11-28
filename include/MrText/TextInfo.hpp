#pragma once

#include <string>
#include <string_view>

class TextInfo {
  using Count = unsigned long;

public:
  Count bytes;
  Count chars;
  Count line_breaks;

  TextInfo() noexcept;
  explicit TextInfo(Count bytes = 0, Count = 0, Count line_breaks = 0) noexcept;
  explicit TextInfo(std::string_view text = std::string()) noexcept;

  auto operator+=(const TextInfo &rval) -> void;
  auto operator-=(const TextInfo &rval) -> void;

  auto operator+(const TextInfo &rval) -> TextInfo;
  auto operator-(const TextInfo &rval) -> TextInfo;

  void count_line_breaks(const std::string_view text) noexcept;
  bool contains_line_break() const noexcept;
};

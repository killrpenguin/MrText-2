#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
using buffer_type = std::array<char, 4>;

template <typename Val_T> class ArrayBuffer {

public:
  ArrayBuffer();
  ArrayBuffer(buffer_type input);

  char operator[](int idx);

  buffer_type inner{};
  unsigned long pos{};

  buffer_type::iterator begin() noexcept;
  buffer_type::iterator end() noexcept;

  constexpr buffer_type::const_iterator cbegin() const noexcept;
  constexpr buffer_type::const_iterator cend() const noexcept;

  constexpr buffer_type::const_reverse_iterator rbegin() const noexcept;
  constexpr buffer_type::const_reverse_iterator rend() const noexcept;

  buffer_type* pInner() noexcept;
  bool valid_char() const noexcept;
  bool empty() const noexcept;
  void reset() noexcept;
  char *next() noexcept;
};

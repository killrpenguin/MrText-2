
#include "MrText/ArrayBuffer.hpp"

#include <array>
#include <iostream>
#include <stdexcept>

template <typename Val_T> ArrayBuffer<Val_T>::ArrayBuffer() = default;

template <typename Val_T>
ArrayBuffer<Val_T>::ArrayBuffer(buffer_type input) : inner{input}, pos{0} {}

template <typename Val_T>
auto ArrayBuffer<Val_T>::valid_char() const noexcept -> bool {
  return inner[pos] != 0;
}

template <typename Val_T>
auto ArrayBuffer<Val_T>::empty() const noexcept -> bool {
  return inner.begin() == inner.end();
}

template <typename Val_T> void ArrayBuffer<Val_T>::reset() noexcept {
  if (inner[0] != 0) {
	pos = 0;
	inner.fill(0);
  }
}

template <typename Val_T>
auto ArrayBuffer<Val_T>::pInner() noexcept -> buffer_type * {
  return &inner;
}

template <typename Val_T> auto ArrayBuffer<Val_T>::operator[](int idx) -> char {
  char ret_val{};
  try {
    ret_val = inner.at(idx);
  } catch (const std::out_of_range &oor) {
    std::cerr << "Out of Range error: " << oor.what() << '\n';
  }
  return ret_val;
}

template <typename Val_T> auto ArrayBuffer<Val_T>::next() noexcept -> char * {
  if (inner.size() == pos) {
    pos = 0;
  }
  try {
    inner.at(pos);
  } catch (const std::out_of_range &oor) {
    std::cerr << "Out of Range error: " << oor.what() << '\n';
  }
  char *p_ret_val = nullptr;
  p_ret_val = &inner[pos];

  pos++;
  return p_ret_val;
}

template <typename Val_T>
auto ArrayBuffer<Val_T>::begin() noexcept -> buffer_type::iterator {
  return inner.begin();
}

template <typename Val_T>
auto ArrayBuffer<Val_T>::end() noexcept -> buffer_type::iterator {
  return inner.end();
}

template <typename Val_T>
constexpr auto ArrayBuffer<Val_T>::rbegin() const noexcept
    -> buffer_type::const_reverse_iterator {
  return inner.rbegin();
}

template <typename Val_T>
constexpr auto ArrayBuffer<Val_T>::rend() const noexcept
    -> buffer_type::const_reverse_iterator {
  return inner.rend();
}

template <typename Val_T>
constexpr auto ArrayBuffer<Val_T>::cbegin() const noexcept
    -> buffer_type::const_iterator {
  return inner.cbegin();
}

template <typename Val_T>
constexpr auto ArrayBuffer<Val_T>::cend() const noexcept
    -> buffer_type::const_iterator {
  return inner.cend();
}

template class ArrayBuffer<char>;

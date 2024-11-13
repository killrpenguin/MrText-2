#include "MrText/ArrayBuffer.hpp"

#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>

template <typename Val_T>
buffer_type::iterator ArrayBuffer<Val_T>::begin() noexcept {
  return inner.begin();
}

template <typename Val_T>
buffer_type::iterator ArrayBuffer<Val_T>::end() noexcept {
  return inner.end();
}

template <typename Val_T>
constexpr buffer_type::const_reverse_iterator
ArrayBuffer<Val_T>::rbegin() const noexcept {
  return inner.rbegin();
}

template <typename Val_T>
constexpr buffer_type::const_reverse_iterator
ArrayBuffer<Val_T>::rend() const noexcept {
  return inner.rend();
}

template <typename Val_T>
constexpr buffer_type::const_iterator
ArrayBuffer<Val_T>::cbegin() const noexcept {
  return inner.cbegin();
}

template <typename Val_T>
constexpr buffer_type::const_iterator
ArrayBuffer<Val_T>::cend() const noexcept {
  return inner.cend();
}

template <typename Val_T>
ArrayBuffer<Val_T>::ArrayBuffer(buffer_type input) noexcept
    : inner(input), pos(0) {}

template <typename Val_T>
bool ArrayBuffer<Val_T>::valid_char() const noexcept {
  return inner[pos] != 0;
}

template <typename Val_T> bool ArrayBuffer<Val_T>::empty() const noexcept {
  return inner.begin() == inner.end();
}

template <typename Val_T> void ArrayBuffer<Val_T>::zero() noexcept {
  inner.fill(0);
}

template <typename Val_T> char ArrayBuffer<Val_T>::operator[](int idx) {
  char ret_val{};
  try {
    ret_val = inner.at(idx);
  } catch (const std::out_of_range &oor) {
    std::cerr << "Out of Range error: " << oor.what() << '\n';
  }
  return ret_val;
}

template <typename Val_T> char *ArrayBuffer<Val_T>::next() noexcept {
  if (inner.size() == pos) {
    pos = 0;
  }
  try {
    inner.at(pos);
  } catch (const std::out_of_range &oor) {
    std::cerr << "Out of Range error: " << oor.what() << '\n';
  }
  char *p_ret_val;
  p_ret_val = &inner[pos];

  pos++;
  return p_ret_val;
}

template class ArrayBuffer<char>;

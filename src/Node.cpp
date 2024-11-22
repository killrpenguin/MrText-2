#include "MrText/Node.hpp"
#include "MrText/TextInfo.hpp"
#include <string>
#include <string_view>

template <typename T, int ORDER>
Node<T, ORDER>::Node(const std::string &text) noexcept
    : cur_key_count{0}, leaf{true}, text{text},
      info{TextInfo(std::string_view(text))} {
  for (int i = 0; i < ORDER; i++) {
    children[i] = nullptr;
  }
}

template <typename T, int ORDER>
Node<T, ORDER>::Node(const std::string &text, bool is_leaf) noexcept
    : cur_key_count{0}, leaf{is_leaf}, text{text},
      info{TextInfo(std::string_view(text))} {
  for (int i = 0; i < ORDER; i++) {
    children[i] = nullptr;
  }
}

template <typename T, int ORDER> Node<T, ORDER>::~Node() = default;

template class Node<char, 3>;

#pragma once

#include "MrText/TextInfo.hpp"

#include <memory>
#include <vector>

template <typename T, int ORDER> class Node {
public:
  T keys[ORDER - 1];
  std::shared_ptr<Node> children[ORDER];
  std::shared_ptr<TextInfo> children_info[ORDER];

  // n in tutorial.
  unsigned int cur_key_count;
  bool leaf;

  unsigned int weight{};
  std::string text;
  TextInfo info;

  explicit Node(const std::string &text) noexcept;
  explicit Node(const std::string &text, bool leaf) noexcept;
  ~Node();

  auto is_full() -> bool;
};

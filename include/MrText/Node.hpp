#pragma once

#include "MrText/TextInfo.hpp"

#include <memory>

template <typename T, int ORDER> class Node {
public:
  // std::array<std::shared_ptr<Node>, ORDER - 1> keys;
  T keys[ORDER - 1];
  // std::array<std::shared_ptr<Node>, ORDER> children;
  std::shared_ptr<Node> children[ORDER];
  std::shared_ptr<TextInfo> children_info[ORDER];
  //  std::array<std::shared_ptr<Node>, ORDER> children_info;

  // n in tutorial.
  unsigned int cur_key_count;
  bool leaf;

  unsigned int weight{};
  std::string text;
  TextInfo info;

  explicit Node(const std::string &text) noexcept;
  explicit Node(const std::string &text, bool leaf) noexcept;
  ~Node();
};

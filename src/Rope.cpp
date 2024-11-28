#include "MrText/Rope.hpp"
#include <csignal>
#include <iostream>
#include <memory>
#include <new>

auto Node::is_leaf() const noexcept -> bool { return !line.empty(); }

Node::Node(std::string line) noexcept
    : line{line}, data{0}, weight{line.length()}, parent{nullptr},
      left{nullptr}, right{nullptr} {
  if (!line.empty()) {
    data = TextInfo(line);
  }
}

auto Rope::_shift_internal(const pNode node) noexcept -> pNode {
  pNode new_node{};
  try {
    new_node = std::make_shared<Node>("");
  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed: " << e.what() << '\n';
    std::exit(SIGABRT);
  }
  new_node->weight = node->weight;
  new_node->left = node->left;
  new_node->right = node->right;

  return new_node;
}

auto Rope::_insert(const pNode node, const std::string &new_line) noexcept
    -> void {
  if (node->left == nullptr) {
    node->left = std::make_shared<Node>(new_line);
    root->weight += new_line.length();
  }

  if (new_line.length() < node->weight) {
    if (!node->left->line.empty()) {
      const pNode new_node = _shift_internal(node->left);
      new_node->parent = node->left;
    } else {
      _insert(node->left, new_line);
    }
  } else {
    if (node->right == nullptr) {
      node->right = std::make_shared<Node>(new_line);
      root->weight += new_line.length();
    } else {
      if (!node->right->line.empty()) {
        const pNode new_node = _shift_internal(node->left);
        new_node->parent = node->right;
      } else {
        _insert(node->right, new_line);
      }
    }
  }
}

auto Rope::insert(const std::string new_line) noexcept -> void {
  if (root == nullptr) {
    try {
      root = std::make_shared<Node>("");
    } catch (const std::bad_alloc &e) {
      std::cerr << "Memory allocation failed: " << e.what() << '\n';
      std::exit(SIGABRT);
    }
  }
  _insert(root, new_line);
}

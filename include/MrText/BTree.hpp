#pragma once
#include "MrText/TextInfo.hpp"

#include <memory>
#include <vector>
#include <string>

template <typename T, int ORDER> class BTreeNode {
public:
  T keys[ORDER - 1];
  std::shared_ptr<BTreeNode> children[ORDER];
  std::shared_ptr<TextInfo> children_info[ORDER];

  // n in tutorial.
  unsigned int cur_key_count;
  bool leaf;

  unsigned int weight{};
  std::string text;
  TextInfo info;

  explicit BTreeNode(const std::string &text) noexcept;
  explicit BTreeNode(const std::string &text, bool leaf) noexcept;
  ~BTreeNode();

  auto is_full() -> bool;
};


template <typename T, int ORDER> class BTree {
  using pNode = std::shared_ptr<BTreeNode<T, ORDER>>;

protected:
  pNode root;

  auto split_child(const pNode arg_node, const int idx) noexcept -> void;
  auto insert_non_full(pNode arg_node, T val) noexcept -> void;
  auto fill(pNode arg_node, int idx) noexcept -> void;
  auto merge(pNode arg_node, int idx) noexcept -> void;
  auto remove_from_non_leaf(pNode arg_node, int idx) noexcept -> void;
  auto remove_from_leaf(pNode arg_node, int idx) noexcept -> void;
  auto remove(pNode arg_node, T val) noexcept -> void;

  auto traverse(pNode &root_node) noexcept -> void;

  auto borrow_from_prev(pNode arg_node, int idx) noexcept -> void;
  auto borrow_from_next(pNode arg_node, int idx) noexcept -> void;

  auto search(pNode arg_node, T val) noexcept -> pNode;

  auto get_predecessor(pNode arg_node, int idx) noexcept -> void;
  auto get_successor(pNode arg_node, int idx) noexcept -> void;

public:
  BTree() noexcept;
  BTree(const std::string text) noexcept;

  auto insert_char(T val) noexcept -> void;
  auto traverse_rope() noexcept -> void;
  auto search(T val) noexcept -> pNode;
  auto remove(T val) noexcept -> void;
};

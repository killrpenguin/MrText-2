#pragma once
#include <memory>
#include <string>

template <typename T, int ORDER> class Node;

template <typename T, int ORDER> class Rope {
  using pNode = std::shared_ptr<Node<T, ORDER>>;

protected:
  pNode root;

  auto split_child(const pNode &arg_node, const int idx) noexcept -> void;
  auto insert_non_full(pNode &arg_node, T val) noexcept -> void;
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
  Rope() noexcept;
  Rope(const std::string text) noexcept;

  auto insert_char(T val) noexcept -> void;
  auto traverse_rope() noexcept -> void;
  auto search(T val) noexcept -> pNode;
  auto remove(T val) noexcept -> void;
};

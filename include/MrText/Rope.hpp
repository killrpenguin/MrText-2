#pragma once
#include <memory>
#include <string>

template <typename T, int ORDER> class Node;

template <typename T, int ORDER> class Rope {
  using pNode = std::shared_ptr<Node<T, ORDER>>;

protected:
  pNode root;

  void split_child(const pNode &arg_node, const int idx) noexcept;
  void insert_non_full(pNode &arg_node, T val) noexcept;
  void fill(pNode arg_node, int idx) noexcept;
  void merge(pNode arg_node, int idx) noexcept;

  void remove_from_non_leaf(pNode arg_node, int idx) noexcept;
  void remove_from_leaf(pNode arg_node, int idx) noexcept;
  void remove(pNode arg_node, T val) noexcept;

  void traverse(pNode &root_node) noexcept;

  void borrow_from_prev(pNode arg_node, int idx) noexcept;
  void borrow_from_next(pNode arg_node, int idx) noexcept;

  pNode search(pNode arg_node, T val) noexcept;

  T get_predecessor(pNode arg_node, int idx) noexcept;
  T get_successor(pNode arg_node, int idx) noexcept;

public:
  Rope() noexcept;
  Rope(const std::string text) noexcept;

  void insert_char(T val) noexcept;
  void traverse_rope() noexcept;
  pNode search(T val) noexcept;
  void remove(T val) noexcept;
};

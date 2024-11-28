#include "MrText/BTree.hpp"
#include "MrText/TextInfo.hpp"
#include <string>
#include <string_view>

#include "MrText/RopeConstants.hpp"

#include <bits/stdc++.h>
#include <iostream>
#include <memory>
#include <new>
#include <string>

template <typename T, int ORDER>
BTreeNode<T, ORDER>::BTreeNode(const std::string &text) noexcept
    : keys{0}, children{0}, children_info{0}, cur_key_count{0}, leaf{true},
      text{text}, info{TextInfo(std::string_view(text))} {
  for (int i = 0; i < ORDER; i++) {
    children[i] = nullptr;
  }
}

template <typename T, int ORDER>
BTreeNode<T, ORDER>::BTreeNode(const std::string &text, bool is_leaf) noexcept
    : keys{0}, children{0}, children_info{0}, cur_key_count{0}, leaf{is_leaf},
      text{text}, info{TextInfo(std::string_view(text))} {
  for (int i = 0; i < ORDER; i++) {
    children[i] = nullptr;
  }
}

template <typename T, int ORDER> BTreeNode<T, ORDER>::~BTreeNode() = default;

template <typename T, int ORDER> BTree<T, ORDER>::BTree() noexcept : root{} {
  try {
    root = std::make_unique<BTreeNode<T, ORDER>>("");
  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed: " << e.what() << '\n';
  }
}

template <typename T, int ORDER>
void BTree<T, ORDER>::traverse_rope() noexcept {
  if (root != nullptr) {
    traverse(root);
  }
}

template <typename T, int ORDER>
void BTree<T, ORDER>::insert_char(T val) noexcept {
  if (root->cur_key_count == ORDER - 1) {
    pNode node{};
    try {
      node = std::make_unique<BTreeNode<T, ORDER>>("", false);
    } catch (const std::bad_alloc &e) {
      std::cerr << "Memory allocation failed: " << e.what() << '\n';
    }
    node->children[0] = root;
    root = node;
    split_child(node, 0);
    insert_non_full(node, val);
  } else {
    insert_non_full(root, val);
  }
};

template <typename T, int ORDER>
auto BTree<T, ORDER>::split_child(const pNode arg_node, const int idx) noexcept
    -> void {

  pNode target_node = arg_node->children[idx];
  pNode new_node{};
  try {
    new_node = std::make_shared<BTreeNode<T, ORDER>>(std::string(""), false);

  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed: " << e.what() << '\n';
  }

  new_node->cur_key_count = ORDER / 2 - 1;

  for (int iter = 0; iter < ORDER / 2 - 1; iter++) {
    new_node->keys[iter] = target_node->keys[iter + (ORDER / 2)];
  }

  if (!target_node->leaf) {
    for (int iter = 0; iter < ORDER / 2; iter++) {
      new_node->children[iter] = target_node->children[iter + (ORDER / 2)];
    }
  }

  target_node->cur_key_count = ORDER / 2 - 1;

  for (int iter = arg_node->cur_key_count; iter >= idx + 1; iter--) {
    arg_node->children[iter + 1] = arg_node->children[iter];
  }

  arg_node->children[idx + 1] = new_node;

  for (int iter = arg_node->cur_key_count - 1; iter >= idx; iter--) {
    arg_node->keys[iter + 1] = arg_node->keys[iter];
  }

  arg_node->keys[idx] = target_node->keys[(ORDER / 2) - 1];
  ++arg_node->cur_key_count;
}

template <typename T, int ORDER>
auto BTree<T, ORDER>::insert_non_full(pNode arg_node, T val) noexcept -> void {
  int idx = arg_node->cur_key_count - 1;

  if (arg_node->leaf) {
    while (idx >= 0 && val < arg_node->keys[idx]) {
      arg_node->keys[idx + 1] = arg_node->keys[idx];
    }
    arg_node->keys[idx + 1] = val;
    ++arg_node->cur_key_count;
  } else {
    while (idx >= 0 && val < arg_node->keys[idx]) {
      --idx;
    }
    ++idx;
    if (arg_node->children[idx]->cur_key_count == ORDER - 1) {
      split_child(arg_node, idx);
      if (val > arg_node->keys[idx]) {
        ++idx;
      }
    }
    insert_non_full(arg_node->children[idx], val);
  }
}

template <typename T, int ORDER>
auto BTree<T, ORDER>::traverse(pNode &root_node) noexcept -> void {
  unsigned int idx = 0;

  for (idx = 0; idx < root_node->cur_key_count; idx++) {
    if (!root_node->leaf) {
      traverse(root_node->children[idx]);
    }
    std::cout << root_node->keys[idx];
  }
  if (!root_node->leaf) {
    traverse(root_node->children[idx]);
  }
}

template class BTree<int, 3>;

template class BTreeNode<int, 3>;

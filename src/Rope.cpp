#include "MrText/Rope.hpp"
#include "MrText/Node.hpp"
#include "MrText/RopeConstants.hpp"

#include <bits/stdc++.h>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

const int RopeConst::MaxStrLen = 23;

template <typename T, int ORDER> Rope<T, ORDER>::Rope() noexcept : root{} {
  try {
    root = std::unique_ptr<Node<T, ORDER>>(new Node<T, ORDER>(""));
  } catch (const std::bad_alloc &e) {
    std::cerr << "Memory allocation failed: " << e.what() << '\n';
  }
}

template <typename T, int ORDER> void Rope<T, ORDER>::traverse_rope() noexcept {
  if (root != nullptr) {
    traverse(root);
  }
}

template <typename T, int ORDER>
void Rope<T, ORDER>::insert_char(T val) noexcept {
  if (root->cur_key_count == ORDER - 1) {
    pNode node{};
    try {
      node = std::unique_ptr<Node<T, ORDER>>(new Node<T, ORDER>("", false));
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
void Rope<T, ORDER>::split_child(const pNode &arg_node,
                                 const int idx) noexcept {

  pNode const target_node = arg_node->children[idx];
  pNode new_node{};
  try {
    new_node = std::unique_ptr<Node<T, ORDER>>(
        new Node<T, ORDER>(std::string(""), false));

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
void Rope<T, ORDER>::insert_non_full(pNode &arg_node, T val) noexcept {
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
void Rope<T, ORDER>::traverse(pNode &root_node) noexcept {
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

template class Rope<char, 3>;

#include "MrText/BTree.hpp"
#include "MrText/TextInfo.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <utility>

template <std::bidirectional_iterator Itr>
auto BTreeNode::is_full(const Itr &bgn, const Itr &end) noexcept -> bool {
  return std::ranges::all_of(
      bgn, end, [](Pair const &pair) { return pair.second != nullptr; });
}

template <std::bidirectional_iterator Itr>
auto BTreeNode::_sort_arr(const Itr &bgn, const Itr &end) noexcept -> void {
  std::sort(bgn, end, [](const auto &lhs, const auto &rhs) {
    return lhs.first < rhs.first;
  });
}

auto BTreeNode::is_leaf() const noexcept -> bool { return !line.empty(); }

BTreeNode::BTreeNode(std::string new_line) noexcept
    : keys{}, left{}, right{}, line{new_line}, data{TextInfo(new_line)} {}

auto BTreeNode::keys_full() const noexcept -> bool {
  return std::ranges::all_of(keys.cbegin(), keys.cend(),
                             [](Pair pair) { return pair.second != nullptr; });
}

BRope::BRope() noexcept : root{nullptr} {
  try {
    root = std::make_shared<BTreeNode>("");
  } catch (const std::bad_alloc &err) {
    std::cerr << "Memory allocation failed: " << err.what() << '\n';
    std::exit(EXIT_FAILURE);
  }
}

auto BRope::_insert(const pNode &node, Pair &new_key) noexcept -> void {
  if (BTreeNode::is_full(node->keys.begin(), node->keys.end())) {
    _insert_full(node);
  }

  auto is_nullptr = [](const Pair &pair) -> bool {
    return pair.second == nullptr;
  };

  auto *next_empty{
      std::find_if(node->keys.begin(), node->keys.end(), is_nullptr)};

  *next_empty = std::move(new_key);
}

auto BRope::_insert_full(const pNode &node) noexcept -> void {
  std::sort(
      node->keys.begin(), node->keys.end(),
      [](const auto &lhs, const auto &rhs) { return lhs.first < rhs.first; });

  uInt median{};
  if (node->keys.size() % 2 == 0) {
    median = btree::MedAvg;
  } else {
    median = btree::MinChildren;
  }

  uInt idx{0};
  auto *iter = node->keys.begin();

  while (iter != node->keys.end()) {
    if (iter->first == median) {
      idx = 0;
    }
    if (iter->first <= median) {
      if (node->left.size() >= btree::MaxChildren) {
        node->left.at(idx) = std::move(*iter);
        idx++;
      }

    } else {
      if (node->right.size() < btree::MaxChildren) {
        node->right.at(idx) = std::move(*iter);
        idx++;
      }
    }
    iter++;
  }
  node->keys[0] = std::move(node->keys.at(median));
}

auto BRope::insert(const uInt line_num, const std::string &line) -> void {
  pNode new_node{};
  try {
    new_node = std::make_shared<BTreeNode>(line);
  } catch (const std::bad_alloc &err) {
    std::cerr << "Memory allocation failed: " << err.what() << '\n';
    std::exit(EXIT_FAILURE);
  }

  Pair new_key = std::pair(line_num, new_node);

  _insert(root, new_key);
}

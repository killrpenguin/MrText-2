#include "MrText/BTree.hpp"
#include "MrText/TextInfo.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
/*
 * 1. If the tree is empty, allocate a root node and insert the key. -
 * 2. Search the appropriate node for insertion.
 * 3. If the node is full, follow the steps below.
 *  3.1 Insert the Elements in increasing order.
 *  3.2 Now, there are elements greater than its limit. So, split at the median.
 *  3.3 Push the median key upwards and make the elements left of the median the
 * left child. Do the same for the right. 3.4 If the upper node is not full,
 * insert the node in increasing order. 3.5 If the upper node is full, repeat
 * this process.
 */

template <std::bidirectional_iterator Itr>
auto BTreeNode::is_full(const Itr &bgn, const Itr &end) noexcept -> bool {
  return std::ranges::all_of(
      bgn, end, [](Pair const &pair) { return pair.second != nullptr; });
}

template <std::bidirectional_iterator Itr>
auto BTreeNode::is_smallest(const Itr &bgn, const Itr &end,
                            const uInt other) noexcept -> bool {
  return std::ranges::all_of(
      bgn, end, [other](Pair const &pair) { return other < pair.first; });
}

template <std::bidirectional_iterator Itr>
auto BTreeNode::is_empty(const Itr &bgn, const Itr &end) noexcept -> bool {
  return std::ranges::all_of(
      bgn, end, [](Pair const &pair) { return pair.second == nullptr; });
}

template <std::bidirectional_iterator Itr>
auto BTreeNode::next_empty(const Itr &bgn, const Itr &end) -> int {
  const auto result{std::ranges::find_if(
      bgn, end, [](const Pair &pair) { return pair.second == nullptr; })};

  return std::distance(bgn, result);
}

template <std::bidirectional_iterator Itr>
auto BTreeNode::test(const Itr &bgn, const Itr &end) noexcept -> void {
  if (bgn != end) {
  }
}

auto BTreeNode::is_leaf() const noexcept -> bool { return !line.empty(); }

BTreeNode::BTreeNode(std::string new_line) noexcept
    : keys{}, left{}, right{}, line{new_line}, data{TextInfo(new_line)} {}

////////////////////////////////////////////////////////////////////
/*
 *
 */
////////////////////////////////////////////////////////////////////

BRope::BRope() noexcept : root{std::make_unique<BTreeNode>("")} {
  assert(root != nullptr);
}

auto BRope::_insert_full(const pNode &node, Pair new_pair) noexcept -> void {
  auto *bgn{node->keys.begin()};
  auto *const end{node->keys.end()};
  const Pair empty_pair(std::pair(0, std::make_unique<BTreeNode>("")));

  std::sort(bgn, end);

  BTreeNode::test(node->left.begin(), node->left.end());

  if (BTreeNode::is_full(node->right.begin(), node->right.end())) {
    // recursive call?
  } else if (BTreeNode::is_full(node->left.begin(), node->left.end())) {
    // recursive call?
  }

  if (BTreeNode::is_smallest(bgn, end, new_pair.first)) {
    const int empty{
        BTreeNode::next_empty(node->left.begin(), node->left.end())};

    node->left.at(empty) = std::move(new_pair);
  } else {
    const int empty{
        BTreeNode::next_empty(node->right.begin(), node->right.end())};
    node->right.at(empty) = std::move(new_pair);
  }

  uInt cnt{};
  while (bgn != end) {
    if (cnt == btree::Median) {
      cnt = 0;
    }
    if (cnt < btree::Median) {
      const int empty_idx{
          BTreeNode::next_empty(node->left.begin(), node->left.end())};

      std::move(bgn, bgn + 1, node->right.begin() + empty_idx);
    }
    if (cnt >= btree::Median) {
      const int empty_idx{
          BTreeNode::next_empty(node->right.begin(), node->right.end())};

      std::move(bgn, bgn + 1, node->right.begin() + empty_idx);
    }
    cnt++;
    bgn++;
  }
}

auto BRope::_insert(const pNode &node, Pair new_pair) noexcept -> void {
  if (BTreeNode::is_empty(node->left.begin(), node->left.end()) &&
      BTreeNode::is_empty(node->right.begin(), node->right.end())) {

    std::move_backward(node->keys.begin(), node->keys.end() - 1,
                       node->keys.end());

    node->keys.at(0) = std::move(new_pair);
  }
}

auto BRope::insert(const uInt line_num, const std::string &line) -> void {
  Pair new_pair = std::pair(line_num, std::make_unique<BTreeNode>(line));

  if (!BTreeNode::is_full(root->keys.begin(), root->keys.end())) {
    _insert(root, std::move(new_pair));
  } else {
    _insert_full(root, std::move(new_pair));
  }
}

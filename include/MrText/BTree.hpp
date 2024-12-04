#pragma once
#include "MrText/TextInfo.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <iterator>
#include <memory>
#include <utility>

class BTreeNode;

using pNode = std::unique_ptr<BTreeNode>;
using uInt = unsigned long;
using Pair = std::pair<uInt, pNode>;

namespace btree {

constexpr uInt round_up(double x) { return static_cast<uInt>(std::ceil(x)); };

constexpr static double M{3.0};
constexpr static uInt MaxChildren{static_cast<uInt>(M)};
constexpr static uInt MaxKeys{static_cast<uInt>(M - 1)};
constexpr static uInt Median = MaxChildren / 2;

const static uInt MinChildren{round_up(M / 2)};
const static uInt MinKeys{2 * (round_up(M / 2) - 1)};

} // namespace btree

using KeysArr = std::array<Pair, btree::MaxKeys>;
using ChildrenArr = std::array<Pair, btree::MaxChildren>;

////////////////////////////////////////////////////////////////////
/*
 *
 */
////////////////////////////////////////////////////////////////////

class BTreeNode {
public:
  KeysArr keys;

  ChildrenArr left;
  ChildrenArr right;

  std::string line;
  TextInfo data;

  explicit BTreeNode(std::string new_line) noexcept;

  template <std::bidirectional_iterator Itr>
  auto static is_full(const Itr &bgn, const Itr &end) noexcept -> bool;

  template <std::bidirectional_iterator Itr>
  auto static is_empty(const Itr &bgn, const Itr &end) noexcept -> bool;

  template <std::bidirectional_iterator Itr>
  auto static is_smallest(const Itr &bgn, const Itr &end,
                          const uInt other) noexcept -> bool;

  template <std::bidirectional_iterator Itr>
  auto static next_empty(const Itr &bgn, const Itr &end) -> int;
  
  template <std::bidirectional_iterator Itr>
  auto static test(const Itr &bgn, const Itr &end) noexcept -> void;
  
  
  auto is_leaf() const noexcept -> bool;
};

////////////////////////////////////////////////////////////////////
/*
 *
 */
////////////////////////////////////////////////////////////////////

class BRope {
private:
  auto _split_child(const pNode &node, Pair new_pair) noexcept -> void;
  auto static _insert_full(const pNode &node, Pair new_pair) noexcept -> void;

  auto static _insert(const pNode &node, Pair new_pair) noexcept -> void;

public:
  const pNode root;

  explicit BRope() noexcept;

  auto insert(const uInt line_num, const std::string &line) -> void;
};

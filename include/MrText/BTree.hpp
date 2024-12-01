#pragma once
#include "MrText/TextInfo.hpp"
#include <algorithm>
#include <array>
#include <iterator>
#include <memory>
#include <utility>

class BTreeNode;

using pNode = std::shared_ptr<BTreeNode>;
using uInt = unsigned long;
using Pair = std::pair<uInt, pNode>;

template <typename T>
concept Iterable = requires(typename std::iterator_traits<T> i) {
                     i.cbegin();
                     i.cend();
                   };

namespace btree {
const static uInt MaxChildren{3};
constexpr static uint MinChildren{MaxChildren / 2};
constexpr static uInt MinKeys{MaxChildren / 2 - 1};
constexpr static uInt MaxKeys{MaxChildren - 1};
constexpr static uInt MedAvg{MinKeys + MinChildren / 2};
} // namespace btree

using KeysArr = std::array<Pair, btree::MaxKeys>;
using ChildrenArr = std::array<Pair, btree::MaxChildren>;

class BTreeNode {
  template <std::bidirectional_iterator Itr>
  auto _sort_arr(const Itr &bgn, const Itr &end) noexcept -> void;

public:
  KeysArr keys;

  ChildrenArr left;
  ChildrenArr right;

  std::string line;
  TextInfo data;
  explicit BTreeNode(std::string new_line) noexcept;

  template <std::bidirectional_iterator Itr>
  auto static is_full(const Itr &bgn, const Itr &end) noexcept -> bool;

  auto is_leaf() const noexcept -> bool;
  auto keys_full() const noexcept -> bool;
};

class BRope {
private:
protected:
  auto _insert(const pNode &node, Pair &new_key) noexcept -> void;
  auto _insert_full(const pNode &node) noexcept -> void;

public:
  pNode root;

  explicit BRope() noexcept;

  auto insert(const uInt line_num, const std::string &line) -> void;

  auto operator+(BRope other) const noexcept -> BRope;
  auto operator+(BRope other) noexcept -> BRope;

  auto operator+=(BRope other) const noexcept -> void;
  auto operator+=(BRope other) noexcept -> void;

  auto operator[](uInt line_num) const noexcept -> std::string_view;
  auto operator[](uInt line_num) noexcept -> std::string_view;
};

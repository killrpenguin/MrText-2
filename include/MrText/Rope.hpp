#pragma once
// #include "MrText/RopeConstants.hpp"
// #include "MrText/TextInfo.hpp"
// #include <array>
// #include <memory>
// #include <string>
// #include <type_traits>
// class Node;

// using pRNode = std::shared_ptr<Node>;
// using uInt = unsigned long;
// class Node {
// public:
//   std::string line;
//   TextInfo data;

//   uInt weight;
//   pRNode parent;
//   pRNode left;
//   pRNode right;

//   explicit Node(std::string line) noexcept;

//   auto is_leaf() const noexcept -> bool;
// };

// class Rope {
//   auto _insert(const pRNode node, const std::string &new_line) noexcept -> void;
//   auto _shift_internal(const pRNode node) noexcept -> pRNode;

// public:
//   pRNode root{nullptr};

//   auto insert(const std::string new_line) noexcept -> void;
// };

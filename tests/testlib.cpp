#define CATCH_CONFIG_MAIN

#include "TestBuffers.hpp"
#include <MrText/ArrayBuffer.hpp>
#include <MrText/MrText.hpp>
#include <MrText/Rope.hpp>
#include <MrText/TextInfo.hpp>

#include <algorithm>
#include <catch2/catch.hpp>
#include <tuple>

// clang-tidy tests/testlib.cpp -p build -- -isystem build/_deps

TEST_CASE("Check parse_cursor_pos.", "[main]") {
  const MrText editor;
  const TestBufferCin test_buffer("40;120R");

  const std::tuple<unsigned int, unsigned int> ret_val{
      editor.parse_pos(test_buffer.rdbuf())};

  REQUIRE(std::get<0>(ret_val) == 40);
  REQUIRE(std::get<1>(ret_val) == 120);
}

TEST_CASE("Check std::array wrapper.", "[main]") {
  const std::array<char, 4> expected = {'\x1b', '[', 'A', 'B'};
  const std::array<char, 4> zeroed = {0, 0, 0, 0};
  ArrayBuffer<char> arr(expected);

  REQUIRE(*arr.next() == '\x1b');
  REQUIRE(*arr.next() == '[');
  REQUIRE(*arr.next() == 'A');
  REQUIRE(*arr.next() == 'B');

  REQUIRE(*arr.next() == '\x1b');
  REQUIRE(arr[2] == 'A');
  REQUIRE(arr.empty() == false);

  arr.reset();
  REQUIRE(std::equal(arr.begin(), arr.end(), std::begin(zeroed)) == true);
}

TEST_CASE("Check TextInfo operator overloading and constructors.", "[main]") {
  const std::string text_1 = std::string("test\n\n"); // 6
  TextInfo test_obj1(text_1);
  REQUIRE(test_obj1.line_breaks == 2);

  const std::string text_2 = std::string("David\n"); // 6
  TextInfo test_obj2(text_2);
  REQUIRE(test_obj2.line_breaks == 1);

  TextInfo test_add{test_obj1 + test_obj2};
  REQUIRE(test_add.bytes == 12);
  REQUIRE(test_add.chars == 12);
  REQUIRE(test_add.line_breaks == 3);

  test_obj1 += test_add;
  REQUIRE(test_obj1.bytes == 18);
  REQUIRE(test_obj1.chars == 18);
  REQUIRE(test_obj1.line_breaks == 5);

  TextInfo test_subtract{test_add - test_obj1};
  REQUIRE(test_subtract.bytes == 0);
  REQUIRE(test_subtract.chars == 0);
  REQUIRE(test_subtract.line_breaks == 0);

  test_obj2 -= test_subtract;
  REQUIRE(test_obj2.bytes == 6);
  REQUIRE(test_obj2.chars == 6);
  REQUIRE(test_obj2.line_breaks == 1);

  REQUIRE(test_obj2.contains_line_break() == true);
}

TEST_CASE("Check rope insertion.", "[main]") {
  TestBufferCout test_buffer("");
  Rope<char, 3> rope;
  rope.insert_char('1');
  rope.traverse_rope();

  //  INFO(test_buffer.str());

  REQUIRE(test_buffer.str() == "1");
}

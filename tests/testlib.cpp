#define CATCH_CONFIG_MAIN

#include <type_traits>
#include <catch2/catch.hpp>
#include <iostream>
#include <MrText/MrText.hpp>
#include <MrText/ArrayBuffer.hpp>
#include <sstream>

TEST_CASE("Check parse_cursor_pos.", "[main]") {
  MrText editor;
  std::istringstream testBuffer("40;120R");

  std::cin.rdbuf(testBuffer.rdbuf());
  std::streambuf *buffer_p = testBuffer.rdbuf();
  std::tuple<unsigned int, unsigned int> ret_val{editor.parse_pos(buffer_p)};

  REQUIRE(std::get<0>(ret_val) == 40);
  REQUIRE(std::get<1>(ret_val) == 120);
}

TEST_CASE("Check parse key.", "[main]") {
  MrText editor;
  key ret_val = editor.parse_key();
  REQUIRE(ret_val == key::null);
}

TEST_CASE("Check utf8 array.", "[main]") {
  std::array<char, 4> expected = {'\x1b', '[', 'A', 'B'};
  std::array<char, 4> zeroed = {0, 0, 0, 0};
  ArrayBuffer<char> arr(expected);

  REQUIRE(*arr.next() == '\x1b');
  REQUIRE(*arr.next() == '[');
  REQUIRE(*arr.next() == 'A');
  REQUIRE(*arr.next() == 'B');
  
  REQUIRE(*arr.next() == '\x1b');
  REQUIRE(arr[2] == 'A');
  REQUIRE(arr.empty() == false);

  arr.zero();
  REQUIRE(std::equal(arr.begin(), arr.end(), std::begin(zeroed)) == true);
}

//  Clear buffer with these two member functions between asserts.
//  testBuffer.str("");
//  testBuffer.clear();
// std::ostringstream testBuffer{};
// std::cout.rdbuf(testBuffer.rdbuf());
// std::string line{"this is a test"};

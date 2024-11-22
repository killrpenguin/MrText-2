#include <iostream>
#include <memory>
#include <sstream>

class TestBufferCout {
  std::ostringstream temp_buffer;
  std::streambuf *backup_buffer;

public:
  TestBufferCout()
      : temp_buffer{std::string("")}, backup_buffer{std::cout.rdbuf()} {
    std::cout.rdbuf(temp_buffer.rdbuf());
  }

  TestBufferCout(const std::string &expected)
      : temp_buffer{expected}, backup_buffer{std::cout.rdbuf()} {
    std::cout.rdbuf(temp_buffer.rdbuf());
  }
  ~TestBufferCout() { std::cout.rdbuf(backup_buffer); }

  static auto rdbuf() -> std::streambuf * { return std::cin.rdbuf(); }
  auto str() const -> std::string { return temp_buffer.str(); }
};

class TestBufferCin {
  std::istringstream temp_buffer;
  std::streambuf *backup_buffer;

public:
  TestBufferCin()
      : temp_buffer{std::string("")}, backup_buffer{std::cout.rdbuf()} {
    std::cin.rdbuf(temp_buffer.rdbuf());
  }

  TestBufferCin(const std::string &expected)
      : temp_buffer{expected}, backup_buffer(std::cout.rdbuf()) {
    std::cin.rdbuf(temp_buffer.rdbuf());
  }
  ~TestBufferCin() { std::cin.rdbuf(backup_buffer); }

  auto str() const -> std::string { return temp_buffer.str(); }
  static auto rdbuf() -> std::streambuf * { return std::cin.rdbuf(); }
};

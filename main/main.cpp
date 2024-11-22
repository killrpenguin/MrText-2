#include "MrText/MrText.hpp"
#include <exception>
#include <iostream>

auto main(int argc, char *argv[]) -> int {
  try {
    if (argc > 1) {
      MrText editor(argv[1]);
      editor.run();
    } else {
      MrText editor;
      editor.run();
    }
  } catch (const std::exception &err) {
    std::cerr << err.what() << '\n';
    return -1;
  }
  return 0;
}

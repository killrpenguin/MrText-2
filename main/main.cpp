#include "MrText/MrText.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) {
  try {
    if (argc > 1) {
      MrText editor(argv[1]);
      editor.run();
    } else {
      MrText editor;
      editor.run();
    }
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    return -1;
  }
  return 0;
}

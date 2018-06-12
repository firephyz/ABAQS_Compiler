#include <iostream>

int main(int argc, char * argv[]) {

  if(argc != 2) {
    std::cerr << "Usage: abaqs <source file>" << std::endl;
    return 1;
  }

  try {
    std::cout << "Hello world" << std::endl;
  } catch (...) {
    std::cerr << "Unknown exception.";
    return 1;
  }

  return 0;
}

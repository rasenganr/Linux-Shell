#include <cstdlib>
#include <iostream>

int main(int argc, char ** argv) {
  for (int i = 1; i < argc; ++i) {
    std::cout << i << "-th argument: " << argv[i] << std::endl;
  }
  return EXIT_SUCCESS;
}

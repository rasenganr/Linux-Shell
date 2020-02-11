#include <cstdlib>
#include <iostream>

int main(void) {
  std::cout << "PATH: " << getenv("PATH") << std::endl;
  std::cout << "somevar: " << getenv("somevar") << std::endl;
  return EXIT_SUCCESS;
}

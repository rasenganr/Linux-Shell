#include <cstdlib>
#include <iostream>
#include <string>

int main(void) {
  std::string curr;

  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  std::cout << "~~~~~~~~~~Testing redirection~~~~~~~~~~" << std::endl;
  std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
  std::cerr << "~~~~~~~~Testing cerr redirection~~~~~~~" << std::endl;
  std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

  while (std::getline(std::cin, curr)) {
    std::cout << "---" << curr << std::endl;
    std::cerr << "***" << curr << std::endl;
  }
  return EXIT_SUCCESS;
}

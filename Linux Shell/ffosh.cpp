// Name: Hancong Wang
// NetID: hw243
//
// This program establishes a command shell
// When you want to run the shell, please type "./ffosh"
// There is no argument for the command shell program itself (you can add some but there will be no effect)
// Please input "exit" or EOF (ctrl+D) to exit the command shell program
// For each program using this command shell, please input the command with arguments after "~/ffosh$"
//
// Functionality:
// (1) Run executable files
// (2) Deal with command line arguments
// (3) Search for indicated programs
// (4) Change working directory
// (5) Operate on variables
// (6) Implement redirection

#include "myFunc.hpp"

/**--Main function
  * Simply call the function "myShell"
  * Check if the shell works just fine
--**/
int main(void) {
  return myShell() ? EXIT_SUCCESS : EXIT_FAILURE;
}

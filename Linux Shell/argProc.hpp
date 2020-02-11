// This hpp file is the definitions for classes
#ifndef H_ARGPROC
#define H_ARGPROC

#include <string.h>

#include <cstdlib>
#include <iostream>
#include <map>

// Arguments passed from command line
class Arguments {
 private:
  std::string command;
  char * path;
  char ** argv;
  int numArgs;

 public:
  // Default constructor
  Arguments() : path(NULL), argv(NULL), numArgs(0) {}
  // Constructor
  Arguments(std::string c, char * p, char ** a, int n) :
      command(c),
      path(p),
      argv(a),
      numArgs(n) {}
  // Copy constructor
  Arguments(const Arguments & rhs);
  // Assignment operator
  Arguments & operator=(const Arguments & rhs);
  // Destructor
  ~Arguments();
  // Get the whole command line
  std::string getCommand() { return command; }
  // Get the path with the command
  char * getPath() { return path; }
  // Get the arguments
  char ** getArgv() { return argv; }
  void redirect(int pos);
};

class ShellVar {
 private:
  std::map<std::string, std::string> vars;
  int numVar;

 public:
  ShellVar(char ** env);
  // Check whether a var-value pair exists
  bool checkVar(std::string var);
  // Get the corresponding value of a var
  std::string getVar(std::string var);
  // Set a variable with a value
  // If already exists, update its value
  void setVar(std::string var, std::string value);
  // Reverse a variable
  void revVar(std::string var);
  // Export a variable to child processes' environment list
  // If already exists, update its value
  void exportVar(std::string var, char **& env);
  // Print a variable and its value
  void printVar(std::string var);
};

#endif

// This cpp file is implementation of methods for classes

#include "argProc.hpp"

/*---------------Arguments---------------*/
// Constructor
Arguments::Arguments(const Arguments & rhs) : path(NULL), argv(NULL), numArgs(0) {
  command = rhs.command;
  argv = new char *[rhs.numArgs];
  for (int i = 0; i < rhs.numArgs; ++i) {
    argv[i] = strdup(rhs.argv[i]);
  }
  path = argv[0];
  numArgs = rhs.numArgs;
}

// Assignment operator
Arguments & Arguments::operator=(const Arguments & rhs) {
  Arguments * temp = new Arguments(rhs);
  std::swap(this->command, temp->command);
  std::swap(this->path, temp->path);
  std::swap(this->argv, temp->argv);
  std::swap(this->numArgs, temp->numArgs);
  return *this;
}

// Destructor
Arguments::~Arguments() {
  for (int i = 0; i < numArgs; ++i) {
    free(argv[i]);
  }
  delete[] argv;
}

// Erase the redirection operator and the redirected file name
void Arguments::redirect(int pos) {
  // Invalid position
  if (pos < 0 || pos >= numArgs) {
    std::cerr << "Invalid redirection position!" << std::endl;
    throw std::exception();
  }
  // Redirection operator
  char * op = argv[pos];
  // Redirected file
  char * filename = argv[pos + 1];
  for (int i = pos; i < numArgs - 2; ++i) {
    argv[i] = argv[i + 2];
  }
  free(op);
  free(filename);
}

/*---------------ShellVar---------------*/
// Constructor
ShellVar::ShellVar(char ** env) : numVar(0) {
  // Set current environment variables
  while (env[numVar] != NULL) {
    std::string temp(env[numVar]);
    size_t eqPos = temp.find_first_of('=');
    std::string first = temp.substr(0, eqPos);
    std::string second = temp.substr(eqPos + 1, temp.length() - 1 - eqPos);
    vars[first] = second;
    ++numVar;
  }
}

// Check whether a var-value pair exists
bool ShellVar::checkVar(std::string var) {
  if (vars.find(var) != vars.end()) {
    return true;
  }
  else {
    return false;
  }
}

// Get the corresponding value of a var
std::string ShellVar::getVar(std::string var) {
  if (vars.find(var) == vars.end()) {
    std::cerr << "No such var!" << std::endl;
    throw std::exception();
  }
  return vars[var];
}

// Set a variable with a value
// If already exists, updata its value
void ShellVar::setVar(std::string var, std::string value) {
  vars[var] = value;
  ++numVar;
}

// Reverse a variable
void ShellVar::revVar(std::string var) {
  // No such variable
  if (vars.find(var) == vars.end()) {
    std::cerr << "There is no such var!" << std::endl;
    throw std::exception();
  }
  // Reverse the corresponding value string
  int len = vars[var].length();
  int i = 0, j = len - 1;
  while (i < j) {
    std::swap(vars[var][i++], vars[var][j--]);
  }
}

// Export a variable to child processes' environment list
// If already exists, update its value
void ShellVar::exportVar(std::string var, char **& env) {
  // No such variable
  if (vars.find(var) == vars.end()) {
    std::cerr << "There is no such var!" << std::endl;
    throw std::exception();
  }
  // Create a new pair with "="
  std::string temp = var + "=" + vars[var];
  char * expVar = strdup(temp.c_str());
  int len = 0;
  int isFound = 0;
  while (env[len] != NULL) {
    std::string curr(env[len]);
    // This variable already exists
    // Update its value
    size_t pos = curr.find_first_of('=');
    if (curr.substr(0, pos) == var) {
      free(env[len]);
      env[len] = expVar;
      isFound = 1;
    }
    ++len;
  }
  // New var-value pair
  if (isFound == 0) {
    env = (char **)realloc(env, (len + 2) * sizeof(*env));
    env[len] = expVar;
    env[len + 1] = NULL;
  }
}

// Print a variable and its value
void ShellVar::printVar(std::string var) {
  std::cout << var << " = " << vars[var] << std::endl;
}

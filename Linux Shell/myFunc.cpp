//This cpp file is the implementation for functions
#include "myFunc.hpp"

/**--Throw an exception
 * Print the error message
 * Throw an exception
--**/
void except(std::string str) {
  std::cerr << str << std::endl;
  throw std::exception();
}

/**--Erase backslashes
 * Erase functional backslashes
 * Leave unfunctional backslashes untouched
--**/
void erasebs(std::string & str) {
  for (unsigned long i = 0; i < str.length(); ++i) {
    if ((str[i] == '\\') && (i < str.length() - 1) && (!isalnum(str[i + 1]))) {
      str.erase(i, 1);
    }
  }
}

/**--Cut out the substring
 * Erase the substring from the original string
 * Return the substring
 * Ask the caller to stop loop
--**/
std::string cutString(std::string & str, size_t head, size_t tail, bool * keepSep) {
  std::string res = str.substr(head, tail);
  if (head == 1) {
    str.erase(0, tail + 2);
  }
  else {
    str.erase(0, tail);
  }
  *keepSep = false;
  return res;
}

/**--Quotation mark processing
 * Keep backslashes if the next character is not functional
 * Contents in quotation marks become a whole argument
 * Characters adjacent to quotation marks become another argument
 * Report an error when there is unclosed quotation mark
--**/
std::string quoteProc(std::string & str, size_t & i, bool * keepSep) {
  std::string res;
  // There is nothing before the quotation mark
  if (i == 0) {
    ++i;  // Skip the current quotation mark
    // Look for the closing quotation mark
    while (i < str.length() && str[i] != '"') {
      if ((str[i] == '\\') && (i < str.length() - 1) && (!isalnum(str[i + 1]))) {
        ++i;
      }
      ++i;
    }
    // Unclosed quotation mark
    if (i == str.length()) {
      except("Unclosed quotation mark!");
    }
    // Legal quotation mark
    res = cutString(str, 1, i - 1, keepSep);
  }
  // Need to cut the string before the quotation mark first
  // Then process the quotation mark in the next loop
  else {
    res = cutString(str, 0, i, keepSep);
  }
  return res;
}

/**--Seperate strings
 * Seperate the input std::string by "delim"
 * Return the first token (char *) seperated from the input string
 * The input string is made shorter
 * Return NULL when the input string is NULL
 * Eliminate redundant whitespaces
 * Keep characters in quotation marks
 * Keep the next character of backslash without its functionality
 * Eliminate backslashes when the next character is not letter nor number
--**/
char * myStrsep(std::string & str, char delim) {
  if (str.length() == 0) {
    return NULL;
  }
  bool keepSep = true;
  size_t i = 0;
  std::string res;
  while (keepSep && i < str.length()) {
    //Encounter a quotation
    if (str[i] == '"') {
      res = quoteProc(str, i, &keepSep);
      break;
    }
    // Encounter a backslash
    else if ((str[i] == '\\') && (i < str.length() - 1) && (!isalnum(str[i + 1]))) {
      str.erase(i++, 1);
    }
    // Encounter a whitespace
    else if (str[i] == delim) {
      res = cutString(str, 0, i, &keepSep);
    }
    // Keep looking for whitespace
    else {
      ++i;
    }
  }
  // Eliminate redundant whitespace characters
  if (!keepSep) {
    size_t next = str.find_first_not_of(delim);
    str.erase(0, next);
  }
  // Reach the end of the string
  else {
    res = str.substr();
    str.erase();
  }
  erasebs(res);
  char * resDup = strdup(res.c_str());
  return resDup;
}

/**--Substitute variables
 * Traverse the command string
 * Find out variables with '$'
 * Search shv, if found, substitute the variable
--**/
void checkvar(std::string & command, ShellVar & shv) {
  unsigned long i = 0;
  while (i < command.length()) {
    // Backslash, skip the next character
    if (command[i] == '\\') {
      i += 2;
    }
    // '$' is found, check variables
    else if (command[i] == '$') {
      unsigned long j = 1;
      std::string res;
      while (i + j < command.length()) {
        std::string temp = command.substr(i + 1, j);
        // Variable found
        if (shv.checkVar(temp)) {
          res = temp;
        }
        ++j;
      }
      command.erase(i, res.length() + 1);
      command.insert(i, shv.getVar(res));
      i += shv.getVar(res).length();
    }
    // Keep looking for variables
    else {
      ++i;
    }
  }
}

/**--Exit shell
 * Print the exit message
 * Stop the loop in main function
--**/
void exitShell(int * keep) {
  std::cout << "~~~~~Thanks for using my shell~~~~~" << std::endl;
  *keep = 0;
  throw std::exception();
}

/**--Print cwd
 * Print the current working directory
--**/
void printcwd(void) {
  char * cwd = new char[256];
  getcwd(cwd, 256);
  std::cout << "ffosh:" << cwd << " $";
  delete[] cwd;
}

/**--Eliminate whitespaces in the head
 * When there are whitespaces in the head of the command
 * Eliminate the whitespaces without changing the rest content
--**/
void erasesp(std::string & str) {
  size_t pos = str.find_first_not_of(' ');
  if (pos == str.npos) {
    str = "";
  }
  else {
    str.erase(0, pos);
  }
}

/**--Get the input arguments
 * Read the input of std::cin to obtain the whole command
 * Seperate each token from the command
 * Store the path and argument information in the object "args"
--**/
Arguments getArg(int * keep, ShellVar & shv) {
  // Print the current working directory
  printcwd();
  std::string command;
  // Exit the shell by input EOF
  if (!std::getline(std::cin, command)) {
    std::cout << std::endl;
    exitShell(keep);
  }
  // Exit the shell by typing "exit"
  if (command == "exit") {
    exitShell(keep);
  }
  // Eliminate whitespaces in front of the whole command
  erasesp(command);
  // No command input
  if (command == "") {
    except("Please at least input something...");
  }
  // Deal with variables
  checkvar(command, shv);
  // Reserve a command for object construction
  std::string reservedCommand(command);
  // Seperate the tokens
  std::vector<char *> vecArgs;
  char * curr = NULL;
  try {
    while ((curr = myStrsep(command, ' ')) != NULL) {
      vecArgs.push_back(curr);
    }
  }
  // Free the alloced memory and re-throw the exception
  catch (std::exception) {
    for (unsigned i = 0; i < vecArgs.size(); ++i) {
      free(vecArgs[i]);
    }
    throw;
  }
  // Create the argvs
  char ** argvs = new char *[vecArgs.size() + 1];
  for (unsigned long i = 0; i < vecArgs.size(); ++i) {
    argvs[i] = vecArgs[i];
  }
  // The argvs should be NULL-terminated
  argvs[vecArgs.size()] = NULL;
  Arguments args(reservedCommand, argvs[0], argvs, (int)(vecArgs.size() + 1));
  return args;
}

/**-- Command not found
 * When the command is not found in the provided path
 * Print the command name with a notification
 * Throw an exception
--**/
void cmdNotFound(std::string command, int isNotFound) {
  if (isNotFound != 0) {
    size_t comPos = command.find_last_of('/');
    command.erase(0, comPos + 1);
    except("Command " + command + " not found");
  }
}

/**--Find the path
 * If there is '/' in the path name, only search the indicated path
 * If there is no '/', searching each directory for the specified program
 * The directories are seperated by ':'
--**/
char * findPath(char * pathName, char * ECE551PATH, int * isRelative) {
  int isNotFound = 1;
  char * currPath;
  std::string command(pathName);
  // The command name contains '/' (absolute path)
  // Only search the indicated path
  if (command.find_first_of('/') != command.npos) {
    *isRelative = 0;
    isNotFound = access(pathName, 0);
    // Check if the command is not found
    cmdNotFound(command, isNotFound);
    return pathName;
  }
  // Search the program in ECE551PATH (relative path)
  std::string path(ECE551PATH);
  char * exePath = NULL;
  command.insert(0, "/");
  while ((currPath = myStrsep(path, ':')) != NULL) {
    // Concatenate path and program name
    std::string temp(currPath);
    temp += command;
    exePath = strdup(temp.c_str());
    // Check if the program was found
    isNotFound = access(exePath, 0);
    free(currPath);
    if (isNotFound == 0) {
      break;
    }
    // Program not found
    else {
      free(exePath);
      exePath = NULL;
    }
  }
  // Check if the command is not found
  cmdNotFound(command, isNotFound);
  return exePath;
}

/**--Print exit information
 * Check and print the status of exit information
 * When the process failed, print the signal or status
--**/
void printExit(int status) {
  if (WIFEXITED(status)) {
    // Succeded
    if (!WEXITSTATUS(status)) {
      std::cout << "Program was successful" << std::endl;
    }
    // Failed
    else {
      std::cout << "Program failed with code " << status << std::endl;
    }
  }
  // Terminated
  else if (WIFSIGNALED(status)) {
    std::cout << "Terminated by signal " << WTERMSIG(status) << std::endl;
  }
}

/**--Change working directory
 * Check if the user want to change working directory
 * If change, set isChange to 1
 * If changing fails, throw an exception
--**/
void chcwd(Arguments & args, int * isChange) {
  if (strcmp(args.getPath(), "cd") == 0) {
    if (args.getArgv()[1] == NULL) {
      except("Please input the target directory");
    }
    if (chdir(args.getArgv()[1]) != 0) {
      except("Failed to change working directory!");
    }
    *isChange = 1;
  }
}

/**--Check variable name
 * Invalid variable name:
   (1) Start with a number
   (2) Contain characters other than numbers, letters and underscores
--**/
void checkvarname(std::string var) {
  if (isdigit(var[0])) {
    except("Invalid variable name!");
  }
  for (size_t i = 0; i < var.length(); ++i) {
    if (!isalnum(var[i]) && var[i] != '_') {
      except("Invalid variable name!");
    }
  }
}

/**--Find the value
 * Skip whitespaces
 * Find the position of the first character of the value
 * Extrac the value string and return it
--**/
std::string findValue(Arguments & args) {
  size_t value_start = args.getCommand().find(args.getArgv()[1]);
  while (args.getCommand()[value_start] != ' ') {
    ++value_start;
  }
  while (args.getCommand()[value_start] == ' ') {
    ++value_start;
  }
  return args.getCommand().substr(value_start);
}

/**--Variable operations
 * Check if the command requires the operation with variables
 * Three functions:
     (1) Set variables with corresponding values
     (2) Export a variable
     (3) Reverse a variable
--**/
void opvar(Arguments & args, ShellVar & shv, int * isVar, char **& myEnviron) {
  // Set variables
  if (strcmp(args.getPath(), "set") == 0) {
    // Incorrect form
    if (args.getArgv()[1] == NULL) {
      except("Please type: set var value");
    }
    else if (args.getArgv()[2] == NULL) {
      except("Please type: set var value");
    }
    // Variable name
    std::string var(args.getArgv()[1]);
    checkvarname(var);
    // Find the value
    std::string value = findValue(args);
    shv.setVar(var, value);
    *isVar = 1;
  }
  // Export variables
  else if (strcmp(args.getPath(), "export") == 0) {
    // Incorrect form
    if (args.getArgv()[1] == NULL) {
      except("Please type: export var");
    }
    shv.exportVar(std::string(args.getArgv()[1]), myEnviron);
    *isVar = 1;
  }
  // Reverse variables
  else if (strcmp(args.getPath(), "rev") == 0) {
    // Incorrect form
    if (args.getArgv()[1] == NULL) {
      except("Please type: rev var");
    }
    shv.revVar(std::string(args.getArgv()[1]));
    *isVar = 1;
  }
}

/**--Check if a file exists
 * Relative path -> search in the current directory
 * Absolute path -> search in the indicated directory
--**/
void checkFile(std::string filename) {
  // Relative path
  if (filename.find('/') == filename.npos) {
    std::string temp = "./" + filename;
    if (access(temp.c_str(), 0)) {
      except("No such file for input redirection!");
    }
  }
  // Absolute path
  else {
    if (access(filename.c_str(), 0)) {
      except("No such file for input redirection!");
    }
  }
}

/**--Check the redirected file
 * When redirecting std::cin, if the file does not exist, throw an exception
 * If there is no file for redirection, throw an exception
--**/
void checkRedirection(Arguments & args) {
  int i = 0;
  while (args.getArgv()[i] != NULL) {
    // std::cin redirection
    if (strcmp(args.getArgv()[i], "<") == 0) {
      // No directed file
      if (args.getArgv()[i + 1] == NULL) {
        except("Please input a file name for redirection");
      }
      // The redirected file does not exist
      else {
        checkFile(std::string(args.getArgv()[i + 1]));
      }
    }
    // std::cout or std::cerr redirection
    else if (strcmp(args.getArgv()[i], ">") == 0 ||
             strcmp(args.getArgv()[i], "2>") == 0) {
      // No redircted file
      if (args.getArgv()[i + 1] == NULL) {
        except("Please input a file name for redirection");
      }
    }
    ++i;
  }
}

/**--Redirection
 * Open the redirected file
 * Replace std::cin, std::cout or std::cerr with the file
 * Close the file after redirection
--**/
void redirect(int stdfile, const char * filename) {
  // Open the redirected file
  // If it does not exist, create one
  int fd = 0;
  int destination = STDIN_FILENO;
  // Redirect std::cin
  if (stdfile == 0) {
    fd = open(filename, O_RDONLY);
  }
  // Redirect std::cerr
  else {
    fd = open(filename,
              O_WRONLY | O_TRUNC | O_CREAT,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    destination = (stdfile == 1) ? STDOUT_FILENO : STDERR_FILENO;
  }
  if (fd == -1) {
    except("Failed to open the redirected file!");
  }
  // dup2 will close std stream
  if (dup2(fd, destination) == -1) {
    except("Failed to replace std stream with the file!");
  }
  // Close the redirected file
  if (close(fd) == -1) {
    except("Failed to close the redirected file!");
  }
}

/**--Redirection and argument process
 * Redirect std::cout, std::cin and std::cerr to specified files
 * "<" is to redirect std::cin
 * ">" is to redirect std::cout
 * "2>" is to redirect std::cerr
--**/
void myRedirection(Arguments & args) {
  int i = 0;
  while (args.getArgv()[i] != NULL) {
    if (strcmp(args.getArgv()[i], "<") == 0) {
      redirect(0, args.getArgv()[i + 1]);
      args.redirect(i);
    }
    else if (strcmp(args.getArgv()[i], ">") == 0) {
      redirect(1, args.getArgv()[i + 1]);
      args.redirect(i);
    }
    else if (strcmp(args.getArgv()[i], "2>") == 0) {
      redirect(2, args.getArgv()[i + 1]);
      args.redirect(i);
    }
    else {
      ++i;
    }
  }
}

/**--Run a program
 * Find the appropriate path for commands without '/'
 * Create a new process
 * Deal with redirection
 * Execute a program in the child process
 * The parent process waits for the child to exit
 * Print corresponding information when child process exits
--**/
void process(Arguments & args,
             ShellVar & shv,
             char **& myEnviron,
             int * isExecve,
             int * keep) {
  int status, isRelative = 1;
  char * ECE551PATH = strdup(shv.getVar(std::string("ECE551PATH")).c_str());
  char * pathName = NULL;
  try {
    pathName = findPath(args.getPath(), ECE551PATH, &isRelative);
  }
  // The command was not found
  // Free the malloced string ECE551PATH
  // Re-throw the exception
  catch (std::exception) {
    free(ECE551PATH);
    throw;
  }
  free(ECE551PATH);
  pid_t pid_child, pid_temp;
  // Create a new process
  pid_child = fork();
  // Child process
  if (pid_child == 0) {
    try {
      // Deal with redirection
      myRedirection(args);
    }
    catch (std::exception) {
      exit(EXIT_FAILURE);
    }
    int exe_status = execve(pathName, args.getArgv(), myEnviron);
    // execve() failing
    if (exe_status == -1) {
      std::cerr << "execve failed" << std::endl;
      *isExecve = 0;
      *keep = 0;
    }
  }
  // Parent process
  else if (pid_child > 0) {
    // wait() failing
    if ((pid_temp = waitpid(pid_child, &status, WUNTRACED)) < 0) {
      except("wait failed");
    }
    // Print the exit status of child process
    printExit(status);
  }
  else {
    // fork() failing
    except("fork failed");
  }
  // Free the strings created by strdup()
  // Only when the path is relative
  if (isRelative) {
    free(pathName);
  }
}

/**--Execute commands
 * Read the arguments
 * Deal with directory changing, variables and redirection
--**/
void execute(ShellVar & shv, char **& myEnviron, int * isExecve, int * keep) {
  // Get the input arguments
  Arguments args = getArg(keep, shv);
  // Change working directory
  int isChange = 0;
  chcwd(args, &isChange);
  // Variable operations
  int isVar = 0;
  opvar(args, shv, &isVar, myEnviron);
  // Redirection check
  checkRedirection(args);
  // Normal commands without changing directory and variables
  if (!isChange && !isVar) {
    process(args, shv, myEnviron, isExecve, keep);
  }
}

/**--Environment copy
 * Copy an environment list
--**/
char ** envcpy(char ** env) {
  char ** myEnv = NULL;
  int i = 0;
  while (env[i] != NULL) {
    myEnv = (char **)realloc(myEnv, (i + 1) * sizeof(*myEnv));
    myEnv[i] = strdup(env[i]);
    ++i;
  }
  myEnv = (char **)realloc(myEnv, (i + 1) * sizeof(*myEnv));
  myEnv[i] = NULL;
  return myEnv;
}

/**--Free evironment variable list
 * Free myEnviron when the shell exits
--**/
void freeEnv(char ** myEnviron) {
  int i = 0;
  while (myEnviron[i] != NULL) {
    free(myEnviron[i++]);
  }
  free(myEnviron);
}

/**--My shell
 * Copy the environment list
 * Keep asking for commands
 * Exit the shell when the user types "exit" or EOF
--**/
bool myShell(void) {
  ShellVar shv(environ);
  char ** myEnviron = envcpy(environ);
  int keep = 1;
  int isExecve = 1;
  // Set the initial path
  shv.setVar(std::string("ECE551PATH"), getenv("PATH"));
  while (keep) {
    try {
      execute(shv, myEnviron, &isExecve, &keep);
    }
    // Something was wrong with the previous command
    // Ask for the next command
    catch (std::exception) {
    }
  }
  freeEnv(myEnviron);
  return (isExecve == 0) ? false : true;
}

// This hpp file is the definition for functions
#ifndef H_MYFUNC
#define H_MYFUNC
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fstream>
#include <stdexcept>
#include <vector>

#include "argProc.hpp"

void except(std::string str);
void erasebs(std::string & str);
std::string cutString(std::string & str, size_t head, size_t tail, bool * keepSep);
std::string quoteProc(std::string & str, size_t & i, bool * keepSep);
char * myStrsep(std::string & str, char delim);
void checkvar(std::string & command, ShellVar & shv);
void exitShell(int * keep);
void printcwd(void);
void erasesp(std::string & str);
Arguments getArg(int * keep, ShellVar & shv);
void cmdNotFound(std::string command, int isNotFound);
char * findPath(char * pathName, char * ECE551PATH, int * isRelative);
void printExit(int status);
void chcwd(Arguments & args, int * isChange);
std::string findValue(Arguments & args);
void opvar(Arguments & args, ShellVar & shv, int * isVar, char **& myEnviron);
void checkRedirection(Arguments & args);
void redirect(int stdfile, const char * filename);
void myRedirection(Arguments & args);
bool isPipe(Arguments & args);
void process(Arguments & args,
             ShellVar & shv,
             char **& myEnviron,
             int * isExecve,
             int * keep);
void exePipe(Arguments & args, ShellVar & shv, char **& myEnviron);
void execute(ShellVar & shv, char **& myEnviron, int * isExecve, int * keep);
char ** envcpy(char ** env);
void freeEnv(char ** myEnviron);
bool myShell(void);

#endif

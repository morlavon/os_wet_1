#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"

using namespace std;

#define WHITESPACE " "
#define BACKSLASH "/"

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

string _ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
  return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
  FUNC_ENTRY()
  int i = 0;
  std::istringstream iss(_trim(string(cmd_line)).c_str());
  for(std::string s; iss >> s; ) {
    args[i] = (char*)malloc(s.length()+1);
    memset(args[i], 0, s.length()+1);
    strcpy(args[i], s.c_str());
    args[++i] = NULL;
  }
  return i;

  FUNC_EXIT()
}

int _parsePath(const char* path, char** args) {
  FUNC_ENTRY()
  int i = 0;
  std::istringstream iss(_trim(string(cmd_line)).c_str());
  for(std::string s; iss >> s; ) {
    args[i] = (char*)malloc(s.length()+1);
    memset(args[i], 0, s.length()+1);
    strcpy(args[i], s.c_str());
    args[++i] = NULL;
  }
  return i;

  int i = 0;
  

  FUNC_EXIT()
}

bool _isBackgroundComamnd(const char* cmd_line) {
  const string str(cmd_line);
  return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
  const string str(cmd_line);
  // find last character other than spaces
  unsigned int idx = str.find_last_not_of(WHITESPACE);
  // if all characters are spaces then return
  if (idx == string::npos) {
    return;
  }
  // if the command line does not end with & then return
  if (cmd_line[idx] != '&') {
    return;
  }
  // replace the & (background sign) with space and then remove all tailing spaces.
  cmd_line[idx] = ' ';
  // truncate the command line string up to the last non-space character
  cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h 

//---------------------------------------------------_____Smalll Shell_____---------------------------------------//
SmallShell::SmallShell() {
// TODO: add your implementation
  prompt = "smash> ";
  OLDPWD[0] = NULL;
}

SmallShell::~SmallShell() {
// TODO: add your implementation


}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
	// For example:

string cmd_s = _trim(string(cmd_line));
string firstWord = cmd_s.substr(0, cmd_s.find_first_of(" \n"));

if(firstWord.compare("chprompt") == 0)
{
  return new ChpromptCommand(cmd_line, this);
}
else if(firstWord.compare("showpid") == 0)
{
  return new ShowPidCommand(cmd_line);
}
else if (firstWord.compare("pwd") == 0)
{
  return new GetCurrDirCommand(cmd_line);
}


/*
  if (firstWord.compare("pwd") == 0) {
    return new GetCurrDirCommand(cmd_line);
  }
  else if (firstWord.compare("showpid") == 0) {
    return new ShowPidCommand(cmd_line);
  }
  else if ...
  .....
  else {
    return new ExternalCommand(cmd_line);
  }
  */
  return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
  // TODO: Add your implementation here
  // for example:
  // Command* cmd = CreateCommand(cmd_line);
  // cmd->execute();
  // Please note that you must fork smash process for some commands (e.g., external commands....)
  Command* cmd = CreateCommand(cmd_line);
  if(cmd->isForked() == true)
  {
    //fork process
  }
  else
  {
    //no fork
    cmd->execute();
  }
  
} 


//----Command
//C'tor
Command::Command(const char* cmd_line)
{
  num_of_args = _parseCommandLine(cmd_line, args);
}

//D'tor
Command::~Command()
{
  for (int i = 0; i< COMMAND_MAX_ARGS; i++)
  {
    free (args[i]);
  }
}


//----BuiltinCommand
//C'tor
BuiltInCommand::BuiltInCommand(const char* cmd_line)
:Command(cmd_line)
{
  this->forked = false;
}

//----ExternalinCommand
//C'tor



//----PipeCommand
//----RederectionCommand
//----ChpromptCommand
//C'tor
ChpromptCommand::ChpromptCommand(const char* cmd_line, SmallShell* smash)
:BuiltInCommand(cmd_line)
{
  this->smash = smash;
  
  if(this->args[1] == NULL)
  {
    this->name = "smash> ";
  }
  else
  {
    this->name = this->args[1];
    this->name.append("> ");
  }
}
//D'tor
//Execute
void ChpromptCommand::execute()
{
  //cout<<this->name;
  this->smash->setPrompt(this->name);
  //cout<<this->smash->getPrompt();
}

//----ShowPidCommand
//C'tor
ShowPidCommand::ShowPidCommand(const char* cmd_line)
:BuiltInCommand(cmd_line)
{
  this->pid = getpid();
  
}

void ShowPidCommand::execute()
{
  std::cout << "smash pid is " << this->pid << std::endl;
}

//----GetCurrDirCommand
//C'tor
GetCurrDirCommand::GetCurrDirCommand(const char* cmd_line)
:BuiltInCommand(cmd_line)
{
  path = getcwd(path, COMMAND_ARGS_MAX_LENGTH);
}

void GetCurrDirCommand::execute()
{
  cout<<path<<endl;
}
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
bool is_exit(std::string& input){
  if(0 == input.find("exit"))
  {
    return true;
  }
  return false;
}

void handle_echo(std::string& input){
  if(input.length() >= 5){
    std::cout << input.substr(5) << std::endl;
  }
  else
  {
    std::cout << std::endl;
  }
}

void nop()
{
  /*
  std::string path = std::getenv("PATH");
  std::vector<std::string> paths;
  std::string delimiter = ":";
  size_t pos = 0;
  std::string token;
  while ((pos = path.find(delimiter)) != std::string::npos) {
    token = path.substr(0, pos);
    paths.push_back(token);
    path.erase(0, pos + delimiter.length());
  }
  paths.push_back(path);
  for (std::string& p : paths)
  {
    std::string fullpath = p + "/" + input;
    if(std::filesystem::exists(fullpath))
    {
      std::cout << fullpath << std::endl;
      return;
    }
  }
*/  
}
//input is sub string of the input, with out the command(type and space)
std::string getfilePath(std::string& input)
{
  std::string path = std::getenv("PATH");
  std::stringstream ss(path);
  std::vector<std::string> paths;
  std::string token;
  while(std::getline(ss, token, ':'))
  {
    paths.emplace_back(token);
  }
  for(std::string& p : paths)
  {
    std::string fullpath = p + "/" + input;
    if(std::filesystem::exists(fullpath))
    {
      return std::move(fullpath);
    }
  }
  return std::move(std::string(""));

}
//input is sub string of the input, with out the command(type and space)
bool handlepath(std::string& input)
{
  std::string path = getfilePath(input);
  if(path.length() > 0)
  {
    std::cout << input << " is " << path << std::endl;
    return true;
  }
  return false;
}

bool try_runcommand(std::string& input)
{
  std::string command = input.substr(0, input.find(' ' ));
  std::string argc;
  if(command.length() != input.length())
  {
    argc =  input.substr(input.find(' ') + 1);
  }
  std::string path = getfilePath(command);
  
  if(path.length() > 0)
  {
    std::string executestring = path + " " + argc;
    system(executestring.c_str());
    return true;
  }
  return false;
}

void handle_type(std::string& input)
{ 
  std::vector<std::string> shellbuildin = {"echo", "type", "exit"};
  if(input.length() < 5)
  {
    std::cout << "type: " << "missing argument" << std::endl;
    return;
  }
  std::string sub = input.substr(5);
  for (std::string& command : shellbuildin)
  {
    
    if(0 == sub.find(command))
    {
      std::string sub = input.substr(5);
      std::cout << sub << " is a shell builtin" << std::endl;
      return;
    }
  }
  if(handlepath(sub))
  {
    return;
  }
  std::cout << sub << ": not found" << std::endl;
}
bool is_handled(std::string& input){
  if(0 == input.find("echo"))
  {
    handle_echo(input);
    return true;
  }
  else if(0 == input.find("type"))
  {
    handle_type(input);
    return true;
  }
  else if (try_runcommand(input))
  {
    return true;
  }
  else 
  {
    return false;
  }
}




int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string input;
  
 
  while(true){
    std::cout << "$ ";
    std::getline(std::cin, input);
    if(is_exit(input))
    {
      break;
    }
    else{
       try
      {
      if(is_handled(input))
      {
        ;
      }
      else{
        std::cout << input << ": " << "command not found" << std::endl;
      }
       }catch(std::exception& e)
        {
         printf("%s", e.what());
        }
    }
  }

}

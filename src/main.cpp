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
//input is sub string of the input, with out the command(type and space)
bool handlepath(std::string& input)
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
      std::cout <<  input << " is " << fullpath << std::endl;
      return true;
    }
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
  else return false;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while(true){

    std::cout << "$ ";

    std::string input;
    std::getline(std::cin, input);
    if(is_exit(input))
    {
      break;
    }
    else{
      if(is_handled(input))
      {
        ;
      }
      else{
        std::cout << input << ": " << "command not found" << std::endl;
      }
    }
  }
}

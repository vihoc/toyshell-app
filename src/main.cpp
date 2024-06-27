#include <iostream>
#include <string>
#include <vector>
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

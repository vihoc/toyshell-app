#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
 static const std::vector<std::string> shellbuildin = {"echo", "type", "exit", "pwd", "cd"};

std::vector<std::string> parsertoken(std::string& input, const char& delimiter = ' ')
{
  std::vector<std::string> tokens;
  std::stringstream ss(input);
  std::string token;
  while(getline(ss, token, delimiter))
  {
    tokens.emplace_back(token);
  }
  return std::move(tokens);
}


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
  if(0 == input.length()) 
  {
    std::cout << "empty command" << std::endl;
    return true;
  }
  std::string command = input.substr(0, input.find(' ' ));
  std::string argc;
  if(0 == command.length())
  {
    std::cout << "empty command" << std::endl;
    return true;
  }
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
  if(input.length() < 5) //cut the type and command
  {
    std::cout << "type: " << "missing argument" << std::endl;
    return;
  }
  std::string sub = input.substr(5);
  for (const std::string& command : shellbuildin)
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
 bool handlecd(std::string& input)
 {
    std::string arg = input.substr(3, input.length());
    if(std::string::npos != arg.find(' '))
    {
      std::cout << "cd : missing argument" << std::endl;
      return true;
    }
    if(0 == arg.find('.'))
    {
      //get the token;
      if(1 == arg.length())
      {
        return true;
      }
      std::vector<std::string> tokens = parsertoken(arg, '/');
      auto current_path = std::filesystem::current_path();
      for(std::string& token : tokens)
      {
        if(".." == token)
        {
          current_path = current_path.parent_path();
          std::filesystem::current_path(current_path);
        }
        else if("." == token)
        {
          continue;
        }
        if("~" == token)
        {
          std::filesystem::current_path(std::getenv("HOME"));
        }
        else
        {
          current_path = current_path / token;
          std::filesystem::current_path(current_path);
        }
      }
      return true;
    }
    //hand with the case of cd with abusolute path
    else if(std::filesystem::exists(arg))
    {
      std::filesystem::current_path(arg);
    }
    else
    {
      std::cout << "cd: "<< arg << ": " <<  "No such file or directory" << std::endl;
    }
    return true;
    
 }

bool handbuildin(std::string& input)
{
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
  else if(0 == input.find("pwd"))
  {
    std::cout << std::filesystem::current_path().c_str() << std::endl;
    return true;
  }
  else if(0 == input.find("cd"))
  {
    handlecd(input);
    return true;
  }
  return false;
}

bool is_handled(std::string& input){
  if(handbuildin(input))
  {
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

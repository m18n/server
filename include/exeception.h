#pragma once
#include <iostream>
namespace server {
class NetworkExeption : public std::exception {
 private:
  std::string message;

 public:
  NetworkExeption(std::string message);
  char* what();
};

}  // namespace server

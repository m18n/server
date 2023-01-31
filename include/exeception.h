#pragma once
#include <iostream>
namespace server {
class ServerExeption : public std::exception {
 protected:
  std::string message;

 public:
  virtual char* what();
};
class NetworkExeption : public ServerExeption {
 private:
  std::string message;

 public:
  NetworkExeption(std::string message);
};
class MemoryExeption : public ServerExeption {
 private:
  std::string message;

 public:
  MemoryExeption(std::string message);
};
}  // namespace server

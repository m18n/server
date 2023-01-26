#include"exeception.h"
server::NetworkExeption::NetworkExeption(std::string message) {
    this->message = "Exeption Network: " + message + "\n";
  }

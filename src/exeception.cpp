#include "exeception.h"
char* server::ServerExeption::what() {
  return &message[0];
}
server::NetworkExeption::NetworkExeption(std::string message) {
  this->message = "Exeption Network: " + message + "\n";
}
server::MemoryExeption::MemoryExeption(std::string message) {
  this->message = "Exeption Memory: " + message + "\n";
}
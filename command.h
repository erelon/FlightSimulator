//
// Created by erelon on 19/12/2019.
//

#ifndef UNTITLED3__COMMAND_H_
#define UNTITLED3__COMMAND_H_
#include <string>

class Command {
 public:
  virtual int execute(std::string *vars) = 0;
};

#endif //UNTITLED3__COMMAND_H_

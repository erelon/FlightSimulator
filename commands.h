//
// Created by erelon on 19/12/2019.
//

#ifndef UNTITLED3__COMMANDS_H_
#define UNTITLED3__COMMANDS_H_

#include "command.h"
#include <string>
#include <unordered_map>
#define READ 0
#define WRITE 1
#define NONE -1

struct vars {
  std::string name;
  double value;
  std::string sim;
  //read or write
  int mode;
};

class allVars {
  //name to vars
  std::unordered_map<std::string, vars> valsMap;
  //adderes to vars for writing
  std::unordered_map<std::string, vars *> adrMap;
  //adderes to vars for reading
  std::unordered_map<std::string, vars *> toReadMap;
 public:
  void addVal(std::string name, double val, int mode, std::string sim);
  bool isAVar(std::string name);
  int UpdateValue(std::string *commands);
  double GetValue(std::string key);
  const std::unordered_map<std::string, vars *> *writeMap();
  std::unordered_map<std::string, vars *> *readMap();
  const std::unordered_map<std::string, vars> *varMap();
};

class OpenServerCommand : public Command {
  int port;
  std::unordered_map<std::string, vars *> *readMap;
 public:
  OpenServerCommand(std::unordered_map<std::string, vars *> *map) {
    this->readMap = map;
  }
  int execute(std::string *vars);
};

class ConnectCommand : public Command {
  std::string ip;
  const std::unordered_map<std::string, vars *> *updateSim;
  int port;
 public:
  ConnectCommand(const std::unordered_map<std::string, vars *> *update) {
    this->updateSim = update;
  }
  int execute(std::string *vars);
};

class DefineVarCommand : public Command {
  allVars *allVar;
 public:
  DefineVarCommand(allVars *ptr);
  int execute(std::string *vars);
};

class Print : public Command {
  allVars *valsMap;
 public:
  Print(allVars *all_vars) {
    valsMap = all_vars;
  }
  int execute(std::string *vars);
};

class Sleep : public Command {
  int time;
 public:
  Sleep() {}
  int execute(std::string *vars);
};

void closeConnctions();

#endif //UNTITLED3__COMMANDS_H_

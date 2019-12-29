//
// Created by erelon on 19/12/2019.
//

#ifndef UNTITLED3__COMMANDS_H_
#define UNTITLED3__COMMANDS_H_
#include "clientserver.h"
#include "command.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <thread>
#include <list>
#include <unordered_map>
#include <functional>
#include <unistd.h>

#define READ 0
#define WRITE 1

using namespace std;

struct vars {
  string name;
  double value;
  string sim;
  //read or write
  int mode;
};

class allVars {
  //name to vars
  unordered_map<string, vars> valsMap;
  //adderes to vars for reading
  unordered_map<string, vars> adrMap;
 public:
  void addVal(string name, double val, int mode, string sim) {
    vars newVal;
    newVal.name = name;
    newVal.sim = sim;
    newVal.value = val;
    newVal.mode = mode;
    valsMap[name] = newVal;
    if (mode == READ)
      adrMap[sim] = newVal;
  }
  bool isAVar(string name) {
    auto ptr = valsMap.find(name);
    if (ptr == valsMap.end())
      return false;
    else return true;
  }
  int UpdateValue(string key, double val) {
    if (valsMap[key].mode == WRITE)
      valsMap[key].value = val;
    return 2;
  }
  double GetValue(string key) {
    return valsMap[key].value;
  }
  const unordered_map<string,vars> *readMap(){
    return &adrMap;
  }
};

class OpenServerCommand : public Command {
  int port;
 public:
  OpenServerCommand() {}
  int execute(string *vars) {
    int port = atoi(vars[0].c_str());
    thread serverThread(serverOpen, port);
    serverThread.detach();

    return 2;
  }
};

class ConnectCommand : public Command {
  string ip;
  int port;
 public:
  ConnectCommand() {
  }
  int execute(string *vars) {
    connectCom(4);
    thread clientThread(connectCom, atoi((vars + 1)->c_str()));
    clientThread.join();
    return 3;
  }
};

class DefineVarCommand : public Command {
  allVars *allVar;
 public:
  DefineVarCommand(allVars *ptr) {
    this->allVar = ptr;
  }
  int execute(string *vars) {
    double value = atof((vars + 1)->c_str());
    int mode;
    vars[1] == "->" ? mode = WRITE : mode = READ;
    allVar->addVal(*vars, value, READ, *(vars + 3));
    return 5;
  }
};

class Print : public Command {
 public:
  Print() {
  }
  int execute(string *vars) {
    string text = vars->substr(1);
    text.pop_back();
    std::cout << text << std::endl;
    return 2;
  }
};

class Sleep : public Command {
  int time;
 public:
  Sleep() {
  }
  int execute(string *vars) {
    //sleep command on thread
    //////////////////////////////////////////vars->c_str() /100
    sleep(0);
    return 2;
  }
};

#endif //UNTITLED3__COMMANDS_H_

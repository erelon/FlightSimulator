//
// Created by erelon on 19/12/2019.
//

#ifndef UNTITLED3__COMMANDS_H_
#define UNTITLED3__COMMANDS_H_
#include "command.h"
#include <string>
#include <stdio.h>
#include <thread>
#include "clinetserver.h"
#include <list>
#include <unordered_map>

#define READ 0
#define WRITE 1

using namespace std;

struct vars {
  double value;
  string sim;
  //read or write
  int mode;
};

class allVars {
  unordered_map<string, vars> valsMap;
 public:
  void addVal(string name, double val, int mode, string sim) {
    vars newVal;
    newVal.sim = sim;
    newVal.value = val;
    newVal.mode = mode;
    valsMap[name] = newVal;
  }
  int isAVar(string name) {
    auto ptr = valsMap.find(name);
    if (ptr == valsMap.end())
      return false;
    else return true;
  }
  void UpdateValue(string key, double val) {
    if (valsMap[key].mode == WRITE)
      valsMap[key].value = val;
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

    return 1;
  }
};

class ConnectCommand : public Command {
  string ip;
  int port;
 public:
  ConnectCommand() {
  }
  int execute(string *vars) {
    thread clientThread(connectCom, atoi(vars->c_str()));
    clientThread.join();
    return 2;
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
    return 4;
  }
};

class Print : public Command {
 public:
  Print() {
  }
  int execute(string *vars) {
    string text = vars->substr(1);
    text.pop_back();
    cout << text << endl;
    return 1;
  }
};

class Sleep : public Command {
  int time;
 public:
  Sleep() {
  }
  int execute(string *vars) {
    //sleep command on thread
    sleep(atoi(vars->c_str()));
    return 2;
  }
};

#endif //UNTITLED3__COMMANDS_H_

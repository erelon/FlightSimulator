//
// Created by erelon on 12/29/19.
//

#include "clientserver.h"
#include "command.h"
#include "commands.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

#define RUN 0
#define PAUSE 1
#define STOP -1

static int threadRunMode = RUN;
allVars *allOfVars;

void allVars::addVal(std::string name, double val, int mode, std::string sim) {
  vars newVal;
  newVal.name = name;
  newVal.sim = sim;
  newVal.value = val;
  newVal.mode = mode;
  valsMap[name] = newVal;
  auto iter = valsMap.find(name);
  if (mode == WRITE)
    adrMap[sim] = &(iter->second);
  else toReadMap[sim] = &(iter->second);
}
bool allVars::isAVar(std::string name) {
  auto ptr = valsMap.find(name);
  if (ptr == valsMap.end())
    return false;
  else return true;
}
int allVars::UpdateValue(std::string *commands) {
  if (valsMap[*commands].mode == WRITE) {
    //commands +1 is a "=" sign
    Interpreter inter;
    string cond = lexCond(&inter, *(commands + 1), *this);
    valsMap[*commands].value = inter.interpret(cond)->calculate();
    //std::atof((*(commands + 1)).c_str());
  }
  return 2;
}
double allVars::GetValue(std::string key) {
  return valsMap[key].value;
}
const std::unordered_map<std::string, vars *> *allVars::writeMap() {
  return &adrMap;
}
std::unordered_map<std::string, vars *> *allVars::readMap() {
  return &this->toReadMap;
}
const std::unordered_map<std::string, vars> *allVars::varMap() {
  return &this->valsMap;
}

int ConnectCommand::execute(std::string *vars) {
  std::thread clientThread(connectCom, atoi((vars + 1)->c_str()), this->updateSim, &threadRunMode);
  clientThread.detach();
  return 3;
}
int OpenServerCommand::execute(std::string *vars) {
  //needs to be condition
  int port = atoi(vars[0].c_str());
  std::thread serverThread(serverOpen, port, this->readMap, &threadRunMode);
  serverThread.detach();

  return 2;
}

DefineVarCommand::DefineVarCommand(allVars *ptr) {
  this->allVar = ptr;
  allOfVars = ptr;
}

int DefineVarCommand::execute(std::string *vars) {
  double value = atof((vars + 1)->c_str());
  int mode;
  if (vars[1] == "->") {
    mode = WRITE;
    allVar->addVal(*vars, value, WRITE, *(vars + 3));
    return 5;
  } else if (vars[1] == "<-") {
    allVar->addVal(*vars, value, READ, *(vars + 3));
    return 5;
  } else {
    Interpreter cond;
    string condition = lexCond(&cond, *(vars + 1), *allVar);
    allVar->addVal(*vars, cond.interpret(condition)->calculate(), NONE, "");
    return 3;
  }
}

int Print::execute(std::string *vars) {
  //needs a condition
  if (vars->find("\"") != std::string::npos) {
    std::string text = vars->substr(1);
    text.pop_back();
    std::cout << text << std::endl;
  } else if (this->valsMap->isAVar(vars->c_str())) {
    std::cout << valsMap->GetValue(vars->c_str()) << std::endl;
  } else {
    Interpreter *sentence = new Interpreter();
    std::string condition = lexCond(sentence, *vars, *allOfVars);
    std::cout << sentence->interpret(condition)->calculate() << std::endl;
  }
  return 2;
}
int Sleep::execute(std::string *vars) {
  //sleep command on thread
  threadRunMode = PAUSE;
  chrono::duration<int> duration(atoi(vars->c_str()) / 1000);
  this_thread::sleep_for(duration);;
  threadRunMode = RUN;
  return 2;
}
void closeConnctions() {
  threadRunMode = STOP;
}

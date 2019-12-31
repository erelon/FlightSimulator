#include "commands.h"
#include "calc/claculator.h"
#include <algorithm>

using namespace std;

void loadCommandMap(unordered_map<string, Command *> *map, allVars *all_vars);
string lexCond(Interpreter *inter, string condition, allVars all_vars);
void unloadCommandMap(unordered_map<string, Command *> *p_map);

void parser(string *commands) {
  unordered_map<string, Command *> commandMap;
  allVars all_vars;
  int j;
  loadCommandMap(&commandMap, &all_vars);

  for (int i = 0; commands[i] != "\0"; i) {
    Command *c = commandMap[commands[i]];
    if (c != nullptr)
      i += c->execute(commands + i + 1);
    else if (all_vars.isAVar(commands[i])) {
      i += all_vars.UpdateValue(commands + i);
    } else if (commands[i].compare("while") == 0) {
      string condition = commands[i + 1];
      //commands[i+2] is '{'
      i += 3;
      int savePlace = i;
      vector<string> loop;
      while (commands[i] != "}") {
        loop.push_back(commands[i]);
        i++;
      }
      //jumping over '}'
      i++;
      //runing the loop
      Interpreter *cond = new Interpreter();
      condition = lexCond(cond, condition, all_vars);
      j = savePlace;
      while (cond->interpret(condition)->calculate()) {
        j = savePlace;
        while (commands[j] != "}") {
          c = commandMap[commands[j]];
          if (c != nullptr)
            j += c->execute(commands + j + 1);
          else if (all_vars.isAVar(commands[j])) {
            j += all_vars.UpdateValue(commands + j);
          }
        }
        condition = lexCond(cond, condition, all_vars);
      }
      delete (cond);
    }
  }
  closeConnctions();
  unloadCommandMap(&commandMap);
}

void unloadCommandMap(unordered_map<string, Command *> *p_map) {
  for (auto item : *p_map)
    delete item.second;
}

void loadCommandMap(unordered_map<string, Command *> *map, allVars *all_vars) {
  OpenServerCommand *openserver = new OpenServerCommand(all_vars->readMap());
  (*map)["openDataServer"] = openserver;
  ConnectCommand *connect = new ConnectCommand(all_vars->writeMap());
  (*map)["connectControlClient"] = connect;
  DefineVarCommand *vardef = new DefineVarCommand(all_vars);
  (*map)["var"] = vardef;
  Print *print = new Print(all_vars);
  (*map)["Print"] = print;
  Sleep *sleep = new Sleep();
  (*map)["Sleep"] = sleep;
}
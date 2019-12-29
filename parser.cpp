#include "commands.h"
#include "calc/claculator.h"
#include "utils.h"
#include <iterator>
#include <algorithm>
#include <sstream>

using namespace std;

void loadCommandMap(unordered_map<string, Command *> *map, allVars *all_vars);
string lexCond(Interpreter *inter, string condition, allVars all_vars);

void lexCond(Interpreter *p_interpreter, string basic_string);
void parser(string *commands) {
  unordered_map<string, Command *> commandMap;
  allVars all_vars;
  int j;
  loadCommandMap(&commandMap, &all_vars);

  for (int i = 0; &commands[i] != nullptr; i) {
    Command *c = commandMap[commands[i]];
    if (c != nullptr)
      i += c->execute(commands + i + 1);
    else if (all_vars.isAVar(commands[i])) {
      i += all_vars.UpdateValue(commands[i], atof(commands[i + 1].c_str()));
    } else if (commands[i].compare("while") == 0) {
      string condition = commands[i + 1];
      //commands[i+2] is '{'
      i += 3;
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
      j = i;
      while (cond->interpret(condition)->calculate()) {
        j = i;
        while (commands[j] != "}") {
          c = commandMap[commands[j]];
          if (c != nullptr)
            j += c->execute(commands + j + 1);
          else if (all_vars.isAVar(commands[j])) {
            j += all_vars.UpdateValue(commands[j], atof(commands[j + 1].c_str()));
          }
          j++;
        }
        i = j;
      }
      delete (cond);
    } else { int x = 5; }
  }
}

string lexCond(Interpreter *inter, string condition, allVars all_vars) {
  vector<string> tokens = split(condition, ' ');

  for (auto token: tokens) {
    if (all_vars.isAVar(token) == true)
      inter->setVariables(token + "=" + to_string(all_vars.GetValue(token)));
  }

  if (condition.find(">=") != string::npos) {
    condition = condition.replace(condition.find(">"), 2, ">") + "|" +
        condition.replace(condition.find(">"), 2, "=");
  }
  if (condition.find("<=") != string::npos) {
    string temp = condition.replace(condition.find("<"), 2, "<") + "|";
    condition.replace(condition.find("<"), 2, "=");
    condition = temp + " " + condition;
  }
  return condition;
}

void loadCommandMap(unordered_map<string, Command *> *map, allVars *all_vars) {
  OpenServerCommand *openserver = new OpenServerCommand();
  (*map)["openDataServer"] = openserver;
  ConnectCommand *connect = new ConnectCommand();
  (*map)["connectControlClient"] = connect;
  DefineVarCommand *vardef = new DefineVarCommand(all_vars);
  (*map)["var"] = vardef;
  Print *print = new Print();
  (*map)["Print"] = print;
  Sleep *sleep = new Sleep();
  (*map)["Sleep"] = sleep;
}
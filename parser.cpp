//
// Created by erelon on 19/12/2019.
//

//#include "command.h"
#include "commands.h"
#include "parser.h"
#include <iterator>

using namespace std;

void loadCommandMap(unordered_map<string, Command *> *map, allVars *all_vars);

void parser(string *commands) {
  unordered_map<string, Command *> commandMap;
  allVars all_vars;
  loadCommandMap(&commandMap, &all_vars);

  for (int i = 0; &commands[i] != nullptr; i++) {
    Command *c = commandMap[commands[i]];
    if (c != nullptr)
      i += c->execute(commands + i + 1);
    else if( all_vars.isAVar(commands[i])){
      all_vars.UpdateValue(commands[i],atof(commands[i+1].c_str()));
    }
  }
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
//
// Created by erelon on 21/12/2019.
//

#ifndef UNTITLED3__CLINETSERVER_H_
#define UNTITLED3__CLINETSERVER_H_
#include <string>
#include "commands.h"

int connectCom(int port, const std::unordered_map<std::string, vars *> *writeMap, int *threadRunMode);
int serverOpen(int port, std::unordered_map<std::string, vars *> *map, int *threadRunMode);

#endif //UNTITLED3__CLINETSERVER_H_

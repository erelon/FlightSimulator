//
// Created by erelon on 12/29/19.
//

#ifndef UNTITLED3__UTILS_H_
#define UNTITLED3__UTILS_H_
#include <string>
#include <vector>
#include "calc/claculator.h"
#include "commands.h"

//https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string &s, char delimiter);
std::string lexCond(Interpreter *inter, std::string condition, allVars all_vars);

#endif //UNTITLED3__UTILS_H_
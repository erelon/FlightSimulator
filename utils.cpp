//
// Created by erelon on 12/29/19.
//

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "commands.h"
#include "calc/claculator.h"

using namespace std;
std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

//input: a pointer to an interpeter, string with a condition that is not well phrased, and allVars class for vars
//output: string with a condition that is well phrased, with variabls from all_vars
string lexCond(Interpreter *inter, string condition, allVars all_vars) {
  string str = condition;
  for (int j = 0; j < str.size() - 1; j++) {
    if ((str[j] == '/') || (str[j] == '+') || (str[j] == '-') || (str[j] == '*') || (str[j] == '(')) {
      if (str[j + 1] != ' ') {
        str.insert(j + 1, " ");
      }
    }
    if (j != 0 && (str[j] == ')') && (str[j - 1] != ' '))
      str.insert(j, " ");
  }
  condition = str;
  vector<string> tokens = split(condition, ' ');

  for (auto token: tokens) {
    if (all_vars.isAVar(token) == true)
      inter->setVariables(token + "=" + to_string(all_vars.GetValue(token)));
  }

  if (condition.find(">=") != string::npos) {
    string temp = condition.replace(condition.find(">"), 2, ">") + "|";
    condition.replace(condition.find(">"), 2, "=");
    condition = temp + " " + condition;
  }
  if (condition.find("<=") != string::npos) {
    string temp = condition.replace(condition.find("<"), 2, "<") + "|";
    condition.replace(condition.find("<"), 2, "=");
    condition = temp + " " + condition;
  }

  for (int i = 0; i < condition.size() - 1; i++) {
    if (condition[i] == '-' && condition[i + 1] != '(') {
      string startSub = condition.substr(0, i + 1);
      startSub.append("1*(");
      string endSub = condition.substr(i + 1);
      int placeOfEndVar = min(endSub.find_first_of(" "), endSub.find_first_of(")"));
      int rem = 0;
      while (placeOfEndVar == 0) {
        endSub = endSub.substr(1);
        placeOfEndVar = min(endSub.find_first_of(" "), endSub.find_first_of(")"));
        rem++;
      }
      placeOfEndVar += rem;
      startSub.append(condition.substr(i + 1, placeOfEndVar) + ")" + endSub.substr(placeOfEndVar - rem));
      condition = startSub;
    }
  }

  return condition;
}
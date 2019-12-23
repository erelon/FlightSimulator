//
// Created by erelon on 19/12/2019.
//

#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>

void addCommand(list<string> *vector, string basic_string);
int numOfBrackets(string basic_string);
using namespace std;

string *lexer(string flyFile) {
  fstream rawData;
  string token;
  list<string> *commands = new list<string>;

  rawData.open(flyFile);
  while (!rawData.eof()) {
    rawData >> token;
    while (numOfBrackets(token) == 1) {
      string temp;
      rawData >> temp;
      token.append(" ");
      token.append(temp);
    }
    if (token.find("=") != string::npos) {
      //read all equsion
      while (rawData.peek() != '\n') {
        string temp;
        rawData >> temp;
        token.append(temp);
        token.append(" ");
      }
      //take out the last " "
      token.pop_back();
      //take out the =
      token = token.substr(1);
      //if there is a { or a } we want it as a separated command
      if (token.find("{") != string::npos ||
          token.find("}") != string::npos) {
        commands->push_back(token.substr(token.length() - 1));
        token.pop_back();
      }
      commands->push_back(token);
    } else addCommand(commands, token);
    if (token == "while") {
      token = "";
      while (rawData.peek() != '\n') {
        string temp;
        rawData >> temp;
        token.append(temp);
        token.append(" ");
      }
      //take out the last " "
      token.pop_back();
      token = token.substr(0, token.length() - 1);
      commands->push_back(token);
      commands->push_back("{");
    }
  }
  rawData.close();
  string *com = new string[commands->size()];
  auto iter = commands->begin();
  int i = 0;
  while (iter != commands->end()) {
    com[i] = *(iter);
    iter++;
    i++;
  }
  return com;
}

int numOfBrackets(string str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++)
    if (str[i] == '(' || str[i] == ')') {
      count++;
    }
  return count;
}

void addCommand(list<string> *vector, string basic_string) {
  int bracketPos = basic_string.find("(");
  int commaPos = basic_string.find(",");

  if (bracketPos != string::npos) {
    basic_string.pop_back();
    vector->push_back(basic_string.substr(0, bracketPos));
    if (commaPos != string::npos) {
      vector->push_back(basic_string.substr(bracketPos + 1, commaPos - bracketPos - 1));
      vector->push_back(basic_string.substr(commaPos + 1));
    } else {
      vector->push_back(basic_string.substr(bracketPos + 1));
    }
  } else vector->push_back(basic_string);
}

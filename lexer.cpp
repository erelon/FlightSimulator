//
// Created by erelon on 19/12/2019.
//

#include <fstream>
#include "utils.h"

using namespace std;

void addCommand(list<string> *vector, string basic_string);
int numOfBrackets(string basic_string);
using namespace std;

string *lexer(string flyFile) {
  fstream rawData;
  list<string> *commands = new list<string>;

  rawData.open(flyFile);
  while (rawData) {
    string token;
    rawData >> token;
    if (token == "")
      continue;
    while (numOfBrackets(token) == 1) {
      string temp;
      rawData >> temp;
      token.append(" ");
      token.append(temp);
    }
    int eqplace = token.find("=");
    if (token.find("Print") != string::npos) {
      commands->push_back("Print");
      string temp = token;
      temp = token.substr(token.find_first_of("(") + 1);
      //take out the ")"
      temp.pop_back();
      commands->push_back(temp);
    } else if (eqplace == 0 || eqplace == token.size() - 1) {
      if (eqplace == token.size() - 1) {
        //take out the "=" in the end:
        token.pop_back();
        //checking that the string had more than just a "="
        if (token != "")
          commands->push_back(token);
      }
      //read all equetion
      while (rawData.peek() != '\n') {
        string temp;
        rawData >> temp;
        token.append(temp);
        token.append(" ");
      }
      //take out the last " "
      token.pop_back();
      if (token[0] == '=') {
        //take out the =
        token = token.substr(1);
      }
      //if there is a { or a } we want it as a separated command
      if (token.find("{") != string::npos ||
          token.find("}") != string::npos) {
        commands->push_back(token.substr(token.length() - 1));
        token.pop_back();
      }
      commands->push_back(token);
    } else if (token.find("=") != string::npos) {
      auto str = split(token, '=');
      commands->push_back(str[0]);
      commands->push_back(str[1]);
    } else addCommand(commands, token);
    if (token == "while" || token == "if") {
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
  commands->push_back("\0");
  string *com = new string[commands->size()];
  auto iter = commands->begin();
  int i = 0;
  while (iter != commands->end()) {
    com[i] = *(iter);
    iter++;
    i++;
  }
  delete commands;
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

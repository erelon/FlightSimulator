//
// Created by erelon on 12/29/19.
//

#ifndef UNTITLED3__UTILS_H_
#define UNTITLED3__UTILS_H_
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstdio>

//https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> split(const std::string &s, char delimiter);
std::vector<std::string> split(const std::string &s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

#endif //UNTITLED3__UTILS_H_
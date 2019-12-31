#include "claculator.h"

//value class
Value::Value(double val) {
  this->val = val;
}
Value::Value(int val) {
  this->val = (double) val;
}
Value::~Value() {
}
double Value::calculate() {
  return val;
}

//binary operation class
Binary::Binary(Expression *left, Expression *right) {
  this->left = left;
  this->right = right;
}
Binary::~Binary() {
  delete (this->left);
  delete (this->right);
}

double Plus::calculate() {
  return left->calculate() + right->calculate();
}
double Minus::calculate() {
  return left->calculate() - right->calculate();
}
double Mul::calculate() {
  return left->calculate() * right->calculate();
}
double Div::calculate() {
  if (right->calculate() == 0)
    throw "divided by zero!";
  return left->calculate() / right->calculate();
}
double AND::calculate() {
  return left->calculate() && right->calculate();
}
double OR::calculate() {
  return left->calculate() || right->calculate();
}
double EQL::calculate() {
  return left->calculate() == right->calculate();
}
double NEQL::calculate() {
  return left->calculate() != right->calculate();
}
double BIG::calculate() {
  return left->calculate() > right->calculate();
}
double SMALL::calculate() {
  return left->calculate() < right->calculate();
}
double EQBIG::calculate() {
  return left->calculate() >= right->calculate();
}
double EQSMALL::calculate() {
  return left->calculate() <= right->calculate();
}
double NOT::calculate() {
  return !(this->exp);
}

//unary operation class
UnaryOperator::UnaryOperator(Expression *exp) {
  this->exp = exp;
}
UnaryOperator::~UnaryOperator() {
  //delete (this->exp);
}

double UPlus::calculate() {
  return this->exp->calculate();
}
UPlus::UPlus(Expression *val) : UnaryOperator(val) {}
UPlus::~UPlus() {
  delete (this->exp);
}
double UMinus::calculate() {
  return -1 * (this->exp->calculate());
}
UMinus::UMinus(Expression *val) : UnaryOperator(val) {}
UMinus::~UMinus() {
  delete (this->exp);
}

//variable class
string Variable::getName() {
  return this->name;
}
double Variable::getValue() {
  return this->value->calculate();
}
Variable::Variable(char name[], int num) {
  string str(name);
  this->name = str;
  this->value = new Value(num);
}
Variable::Variable(char name[], double num) {
  string str(name);
  this->name = str;
  this->value = new Value(num);
}
Variable::Variable(string name, int num) {
  this->name = name;
  this->value = new Value(num);
}
Variable::Variable(string name, double num) {
  if (!((name[0] >= 'A' && name[0] <= 'Z') || (name[0] >= 'a' && name[0] <= 'z') || (name[0] == '_'))) {
    throw "bad input";
  }
  this->name = name;
  this->value = new Value(num);
}
Variable::~Variable() {
  delete (this->value);
}

Variable &Variable::operator++() {
  Value *newVal = new Value(this->value->calculate() + 1);
  delete (this->value);
  this->value = newVal;
  return *this;
}
Variable &Variable::operator--() {
  Value *newVal = new Value(this->value->calculate() - 1);
  delete (this->value);
  this->value = newVal;
  return *this;
}
Variable &Variable::operator++(int) {
  Value *newVal = new Value(this->value->calculate() + 1);
  delete (this->value);
  this->value = newVal;
  return *this;
}
Variable &Variable::operator--(int) {
  Value *newVal = new Value(this->value->calculate() - 1);
  delete (this->value);
  this->value = newVal;
  return *this;
}
Variable &Variable::operator+=(double other) {
  Value *newVal = new Value(this->value->calculate() + other);
  delete (this->value);
  this->value = newVal;
  return *this;
}
Variable &Variable::operator-=(double other) {
  Value *newVal = new Value(this->value->calculate() - other);
  delete (this->value);
  this->value = newVal;
  return *this;
}
double Variable::calculate() {
  return this->value->calculate();
}

//from https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
std::string ReplaceAll(std::string str, const std::string &from, const std::string &to) {
  size_t start_pos = 0;
  while ((str != "") && (start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
  }
  return str;
//
}
Interpreter::~Interpreter() {
}
//input: A string with a (var + "=" + value)
void Interpreter::setVariables(string str) {
  string temp(str);
  try {
    while (!temp.empty()) {
      string varName;
      string varValueStr;
      if ((temp[0] <= '9') && (temp[0] >= '0')) {
        throw temp[0];
      }
      while (temp[0] != '=') {
        varName += temp[0];
        temp.erase(0, 1);
      }
      //clean the '='
      temp.erase(0, 1);
      if (temp[0] == '-') {
        varValueStr += temp[0];
        temp.erase(0, 1);
      }
      while (((temp[0] <= '9') && ((temp[0]) >= '0')) || (temp[0] == '.')) {
        varValueStr += temp[0];
        temp.erase(0, 1);
      }
      if ((temp[0] != ';' && !temp.empty()) || (varValueStr[0] == '.') || (varValueStr.empty()))
        throw "bad input";
      double varValue = stod(varValueStr);
      for (list<Variable>::iterator it = vars.begin(); it != vars.end(); it++) {
        if ((*it).getName() == varName) {
          this->vars.erase(it);
          break;
        }
      }
      this->vars.push_back(*new Variable(varName, varValue));
      //delete ';'
      if (temp[0] == ';')
        temp.erase(0, 1);
    }
  }
  catch (char ch) {
    throw "error";
  }
}
void checkExpression(string str) {
  for (int i = 0; i < str.length() - 1; i++) {
    if ((str[i] == '/' || str[i] == '-' || str[i] == '+' || str[i] == '*') &&
        (str[i + 1] == '/' || str[i + 1] == '-' || str[i + 1] == '+' || str[i + 1] == '*'))
      throw "bad input";
    if (str[i] == '(' && (str[i + 1] == '*' || str[i + 1] == '/'))
      throw "bad input";
    if (str[i] == '-' && !(str[i + 1] <= '9' && str[i + 1] >= '0' || str[i + 1] == '('))
      throw "bad input";
    if ((str[i] >= '0' && str[i] <= '9') && ((str[i + 1] <= 'Z' && str[i + 1] >= 'a') || str[i + 1] == '_'))
      throw "bad input";
  }
}

Expression *Interpreter::interpret(string str) {
  string expression(str);
  //transforming the vars to numbers
  list<Variable>::iterator itr;
  for (itr = this->vars.begin(); itr != this->vars.end(); itr++) {
    if ((*itr).getName() != "") {
      expression = ReplaceAll(expression, (*itr).getName(), to_string((*itr).getValue()));
    }
  }
  expression = ReplaceAll(expression, "-(", "-1*(");
  checkExpression(str);
  return shuttingYard(expression);
}

// Created by erelon on 04/11/2019.
//
//

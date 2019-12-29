//
// Created by erelon on 04/11/2019.
//

#ifndef UNTITLED_EX1_H
#define UNTITLED_EX1_H
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include "Expression.h"
#include "shutting_yard.h"

class Value : public Expression {
 private:
  double val;
 public:
  Value(double val);
  Value(int val);
  ~Value();
  double calculate();

};

class Variable : public Expression {
 private:
  Value *value;
  string name;
 public:
  Variable(char name[], int num);
  Variable(char name[], double num);
  Variable(string name, int num);
  Variable(string name, double num);
  virtual ~Variable();
  Variable &operator++();
  Variable &operator++(int);
  Variable &operator--();
  Variable &operator--(int);
  Variable &operator+=(double other);
  Variable &operator-=(double other);
  string getName();
  double getValue();
  double calculate();
};

class Binary : public Expression {
 protected:
  Expression *left, *right;
 public:
  Binary(Expression *left, Expression *right);
  virtual ~Binary();
};
class UnaryOperator : public Expression {
 protected:
  Expression *exp;
 public:
  UnaryOperator(Expression *exp);
  virtual ~UnaryOperator();
};

class Plus : public Binary {
 public:
  Plus(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class Minus : public Binary {
 public:
  Minus(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class Mul : public Binary {
 public:
  Mul(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class Div : public Binary {
 public:
  Div(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class AND : public Binary {
 public:
  AND(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class OR : public Binary {
 public:
  OR(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class EQL : public Binary {
 public:
  EQL(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class NEQL : public Binary {
 public:
  NEQL(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class BIG : public Binary {
 public:
  BIG(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class SMALL : public Binary {
 public:
  SMALL(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class EQBIG : public Binary {
 public:
  EQBIG(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};
class EQSMALL : public Binary {
 public:
  EQSMALL(Expression *r, Expression *l) : Binary(r, l) {};
  double calculate();
};

class NOT : public UnaryOperator {
 public:
  NOT(Expression *val);
  virtual ~NOT();
  double calculate();
};

class UPlus : public UnaryOperator {
 public:
  UPlus(Expression *val);
  virtual ~UPlus();
  double calculate();
};
class UMinus : public UnaryOperator {
 public:
  UMinus(Expression *val);
  virtual ~UMinus();
  double calculate();
};

class Interpreter {
 private:
  string expression;
  list<Variable> vars;
 public:
  virtual ~Interpreter();
  Expression *interpret(string str);
  void setVariables(string str);
};
#endif //UNTITLED_EX1_H
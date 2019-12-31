#include "shutting_yard.h"

class Members {
 private:
  double num;
  char oper;
  char whatIsIn = '0';
  bool negative = false;
 public:
  Members(double num, bool sign);
  Members(char ch);
  virtual ~Members();
  char getChar();
  double getNum();
  bool isNeg();
};
Members::~Members() {}
bool Members::isNeg() {
  return negative;
}
Members::Members(char ch) {
  oper = ch;
  whatIsIn = 'c';
}
Members::Members(double num, bool sign) {
  this->num = num;
  if (sign)
    negative = !negative;
  whatIsIn = 'n';
}
char Members::getChar() {
  if (whatIsIn == 'c') {
    return oper;
  } else if (whatIsIn == 'n') {
    return '~';
  }
}
double Members::getNum() {
  if (whatIsIn == 'c') {
    return -1;
  } else if (whatIsIn == 'n') {
    return num;
  }
}

bool isOperator(char ch) {
  if ((ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '>') || (ch == '<') || (ch == '=')
      || (ch == '|') || (ch == '&'))
    return true;
  else return false;
}

int operatorImportance(char op) {
  if ((op == '(') || (op == ')'))
    return -1;
  if ((op == '+') || (op == '-'))
    return 0;
  if ((op == '*') || (op == '/'))
    return 1;

  if ((op == '|') || (op == '&'))
    return 2;
  if ((op == '>') || (op == '<') || (op == '='))
    return 3;
}

Expression *sign(Expression *left, char sign, Expression *right) {
  switch (sign) {
    case '+':return new Plus(left, right);
      break;
    case '-': return new Minus(left, right);
      break;
    case '*': return new Mul(left, right);
      break;
    case '/': return new Div(left, right);
      break;
    case '&':return new AND(left, right);
      break;
    case '|':return new OR(left, right);
      break;
    case '=':return new EQL(left, right);
      break;
    case '<':return new SMALL(left, right);
      break;
    case '>':return new BIG(left, right);
      break;
    default:break;
  }
}
Expression *sign(Expression *left, char sign) {
  switch (sign) {
    case '+':return new UPlus(left);
      break;
    case '-': return new UMinus(left);
      break;
    default:break;
  }
}
Expression *toExpression(queue<Members *> postfix) {
  //stack<double> memStack;
  stack < Expression * > result;
  Expression *right, *left;
  Expression *conclusion;
  while (!postfix.empty()) {
    if (postfix.front()->getNum() != -1) {
      //this is a number
      result.push(new Value(postfix.front()->getNum()));
      if (postfix.front()->isNeg()) {
        right = result.top();
        result.pop();
        result.push(new UMinus(right));
      }
    } else if (postfix.front()->getChar() != '~') {
      //this is an operator
      right = result.top();
      result.pop();
      if (!result.empty()) {
        left = result.top();
        result.pop();
        result.push(sign(left, postfix.front()->getChar(), right));
      } else {
        //this is an unary operation
        result.push(sign(right, postfix.front()->getChar()));
      }
    }
    postfix.pop();
  }
  conclusion = result.top();
  result.pop();
  return conclusion;
}

Expression *shuttingYard(const string input) {
  queue < Members * > postfix;
  stack<char> operators;
  char ch;
  int i(0);
  double numOfFracDig(0);
  double num;
  double fraction;
  bool isfraction = false, negVal = false, is0;
  while (i < input.length()) {
    ch = input[i];
    numOfFracDig = 0;
    num = 0;
    fraction = 0;
    isfraction = false;
    is0 = false;
    if (('9' >= ch) && (ch >= '0'))
      is0 = true;
    while ((('9' >= ch) && (ch >= '0')) || (ch == '.')) {
      if (ch == '.') {
        isfraction = true;
        i++;
        ch = input[i];
        continue;
      } else if (ch != '0')
        is0 = false;
      if (isfraction) {
        fraction = (fraction * 10) + (ch - '0');
        numOfFracDig++;
      } else {
        num = (num * 10) + (ch - '0');
      }
      i++;
      ch = input[i];
    }
    if (numOfFracDig != 0) {
      num = num + (fraction / pow(10, numOfFracDig));
    }
    if (num != 0 || is0 == true) {
      postfix.push(new Members(num, negVal));
      negVal = false;
    }
    //in case this is not a number or part of it:
    if (isOperator(ch)) {
      //(i == 0) &&
      if ((i + 1 < input.length()) && (input[i] == '-') && (input[i + 1] != '(')) {
        negVal = true;
        i++;
        continue;
      }
      while ((!operators.empty()) && (operatorImportance(ch) < operatorImportance(operators.top()))) {
        postfix.push(new Members(operators.top()));
        operators.pop();
      }
      operators.push(ch);
    } else if (ch == '(') {
      operators.push(ch);
      if ((i + 1 < input.length()) && (input[i + 1] == '-')) {
        negVal = true;
        i++;
      }
    } else if (ch == ')') {
      while (operators.top() != '(') {
        postfix.push(new Members(operators.top()));
        operators.pop();
      }
      operators.pop();
    }
    i++;
  }
  while (!operators.empty()) {
    postfix.push(new Members(operators.top()));
    operators.pop();
  }
  Expression *exp = toExpression(postfix);
  //cleanup:
  while (!postfix.empty()) {
    Members *mem = postfix.front();
    delete (mem);
    postfix.pop();
  }
  return exp;
}
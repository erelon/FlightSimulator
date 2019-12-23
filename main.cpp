#include <stdio.h>
#include "lexer.h"
#include "parser.h"

using namespace std;
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "error";
    return 1;
  }
  string *commands;
  commands = lexer(argv[1]);
  parser(commands);

  return 0;
}

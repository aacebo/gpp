#include <iostream>

#include "lexer.hpp"

using namespace std;

int main() {
    lexer* lex = new lexer();
    lex->read();
    cout << lex->files.size() << endl;
    return 0;
}

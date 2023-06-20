#include <string>

#include "token.hpp"

using namespace std;

token::token(const type t, const string lex, const int ln) {
    _type_ = t;
    _lex_ = lex;
    _ln_ = ln;
}

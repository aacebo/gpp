#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "scanner.hpp"
#include "error.hpp"
#include "value.hpp"

using namespace std;

namespace parser {
    class Parser {
        Scanner* scanner;
        vector<error::Error> errors;

        public:
            Token* curr;
            Token* prev;

            Parser(const string&);
            ~Parser();

            bool next();
            bool match(Type);
            void consume(Type, const string&);
            void sync();
    };
};

#endif
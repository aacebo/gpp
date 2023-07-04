#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>

using namespace std;

namespace error {
    class Error : public exception {
        public:
            const int ln;
            const int start;
            const int end;
            const string message;

            Error(int, int, int, string);
            Error(const Error&);
            const string what();
    };

    class SyntaxError : public Error { using Error::Error; };
    class RuntimeError : public Error { using Error::Error; };
};

#endif
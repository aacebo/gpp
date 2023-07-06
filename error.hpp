#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>

using namespace std;

namespace error {
    class Error : public exception {
        public:
            int ln;
            int start;
            int end;
            string message;

            Error(int, int, int, string);
            Error(const Error&);

            const string what();
            bool operator<(Error&);
    };

    class SyntaxError : public Error { using Error::Error; };
    class RuntimeError : public Error { using Error::Error; };
};

#endif
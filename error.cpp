#include <string>

#include "error.hpp"

using namespace std;

namespace error {
    Error::Error(
        int ln,
        int start,
        int end,
        string message
    ) : ln(ln), start(start), end(end), message(message) {

    }

    const string Error::what() {
        return "[ln: " + to_string(this->ln) + ", start: " + to_string(this->start) +
            ", end: " + to_string(this->end) + "] -> " + this->message;
    }
};

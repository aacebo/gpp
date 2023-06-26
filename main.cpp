#include "file_reader.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "error.hpp"
#include "interpreter.hpp"

using namespace std;

int main() {
    file_reader::FileReader reader(".");

    for (auto f : reader.get_files()) {
        scanner::Scanner s(f.second);

        for (auto e : s.get_errors()) {
            cout << e.what() << endl;
        }
        
        for (auto t : s.get_tokens()) {
            cout << t.fmt() << endl;
        }

        try {
            parser::Parser p(s.get_tokens());
        
            for (auto e : p.get_errors()) {
                cout << e.what() << endl;
            }

            cout << p.get_statements().size() << endl;
        } catch (error::Error e) {
            cout << e.what() << endl;
        }
    }

    return 0;
}

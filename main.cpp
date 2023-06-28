#include "file_reader.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "error.hpp"

using namespace std;

int main() {
    auto interpreter = new interpreter::Interpreter();
    auto reader = new file_reader::FileReader(".");

    for (auto f : reader->get_files()) {
        auto scanner = new scanner::Scanner(f.second);

        if (scanner->get_errors().size() > 0) {
            for (auto e : scanner->get_errors()) {
                cout << e->what() << endl;
            }

            return -1;
        }

        try {
            auto parser = new parser::Parser(scanner->get_tokens());
        
            if (parser->get_errors().size() > 0) {
                for (auto e : parser->get_errors()) {
                    cout << e->what() << endl;
                }

                return -1;
            }

            try {
                interpreter->run(parser->get_statements());
            } catch (error::RuntimeError* e) {
                cout << e->what() << endl;
                return -1;
            }

            delete parser;
        } catch (error::SyntaxError* e) {
            cout << e->what() << endl;
            return -1;
        }

        delete scanner;
    }

    delete interpreter;
    delete reader;
    return 0;
}

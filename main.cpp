#include "file_reader.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

using namespace std;

int main() {
    auto interpreter = new interpreter::Interpreter();
    auto reader = new file_reader::FileReader(".");

    for (auto f : reader->get_files()) {
        auto scanner = new scanner::Scanner(f.second);

        for (auto e : scanner->get_errors()) {
            cout << e->what() << endl;
        }
        
        for (auto t : scanner->get_tokens()) {
            cout << t->to_string() << endl;
        }

        try {
            auto parser = new parser::Parser(scanner->get_tokens());
        
            if (parser->get_errors().size() > 0) {
                for (auto e : parser->get_errors()) {
                    cout << e->what() << endl;
                }

                return -1;
            }

            interpreter->run(parser->get_statements());
            delete parser;
        } catch (exception e) {
            cout << e.what() << endl;
            return -1;
        }

        delete scanner;
    }

    delete interpreter;
    delete reader;
    return 0;
}

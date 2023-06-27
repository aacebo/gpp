#include "file_reader.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "error.hpp"
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
            cout << t->fmt() << endl;
        }

        try {
            auto parser = new parser::Parser(scanner->get_tokens());
        
            for (auto e : parser->get_errors()) {
                cout << e->what() << endl;
            }

            cout << parser->get_statements().size() << endl;
            delete parser;
        } catch (error::Error e) {
            cout << e.what() << endl;
        }

        delete scanner;
    }

    delete interpreter;
    delete reader;

    return 0;
}

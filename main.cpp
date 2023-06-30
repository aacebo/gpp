#include "file_reader.hpp"
#include "parser.hpp"
#include "error.hpp"

using namespace std;

int main() {
    auto reader = new file_reader::FileReader(".");

    for (auto f : reader->get_files()) {
        auto parser = new parser::Parser(f.second);

        while (parser->next()) {
            cout << parser->curr->to_string() << endl;
        }

        delete parser;
    }

    delete reader;
    return 0;
}

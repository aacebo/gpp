#include "file_reader.hpp"
#include "scanner.hpp"
#include "error.hpp"

using namespace std;

int main() {
    auto reader = new file_reader::FileReader(".");

    for (auto f : reader->get_files()) {
        auto scanner = new scanner::Scanner(f.second);

        for (;;) {
            auto token = scanner->scan();

            if (token.type == token::Type::Eof) {
                break;
            }

            cout << token.to_string() << endl;
        }

        delete scanner;
    }

    delete reader;
    return 0;
}

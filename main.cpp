#include "file_reader.hpp"
#include "scanner.hpp"
#include "error.hpp"

using namespace std;

int main() {
    auto reader = new file_reader::FileReader(".");

    for (auto f : reader->get_files()) {
        auto scanner = new scanner::Scanner(f.second);

        if (scanner->get_errors().size() > 0) {
            for (auto e : scanner->get_errors()) {
                cout << e->what() << endl;
            }

            return -1;
        }

        for (auto t : scanner->get_tokens()) {
            cout << t->to_string() << endl;
        }

        delete scanner;
    }

    delete reader;
    return 0;
}

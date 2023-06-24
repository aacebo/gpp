#include "file_reader.hpp"
#include "scanner.hpp"
#include "parser.hpp"

using namespace std;

int main() {
    file_reader::FileReader reader(".");

    for (auto f : reader.get_files()) {
        scanner::Scanner s(f.second);
        
        for (auto t : s.get_tokens()) {
            cout << t.fmt() << endl;
        }
    }

    return 0;
}

#include "file_reader.hpp"
#include "vm.hpp"
#include "error.hpp"

using namespace std;

int main() {
    auto reader = new file_reader::FileReader(".");
    auto vm = new vm::VM();

    try {
        for (auto file : reader->get_files()) {
            vm->compile(file.second);
        }

        vm->run();
    } catch (vector<error::Error>& errors) {
        for (auto e : errors) {
            cout << e.what() << endl;
        }
    }

    delete vm;
    delete reader;
    return 0;
}

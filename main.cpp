#include "file_reader.hpp"
#include "vm.hpp"
#include "error.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "must specify source path" << endl;
        return -1;
    }

    auto reader = new file_reader::FileReader(argc, argv);
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

        return -1;
    }

    delete vm;
    delete reader;
    return 0;
}

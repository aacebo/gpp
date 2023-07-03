#include "file_reader.hpp"
#include "vm.hpp"
#include "error.hpp"

using namespace std;

int main() {
    auto reader = new file_reader::FileReader(".");
    auto vm = new vm::VM();

    for (auto file : reader->get_files()) {
        vm->compile(file.second);
    }

    vm->run();

    delete reader;
    delete vm;
    return 0;
}

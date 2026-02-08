#include "terminal.hpp"

using namespace alt::io;

int main() {
    terminal t{};
    std::cout << t.set_buffer_size(1, 1).read_line();
    t.restore();
    return 0;
}
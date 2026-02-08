#include "terminal.hpp"

using namespace alt::io;

int main() {
    terminal t{};
    t.write("Write to input stream", in);
    return 0;
}
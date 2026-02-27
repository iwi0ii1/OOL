#include "./string.hpp"

using namespace asl::containers;

void printer(const string& str) {
    for (auto each : str)
        putchar(each);
}

int main() {
    string hello{"hello"};
    printer(hello + "huh");
    return 0;
}
#include "./string.hpp"
#include <iostream>
#include <string>

using namespace asl::containers;

int main() {
    string hello{"hello"};
    hello.erase(hello.begin(), hello.begin() + 4);
    std::cout << std::string(hello.begin().base_ptr, hello.end().base_ptr) << std::endl;
    return 0;
}
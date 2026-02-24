#include "./string.hpp"
#include <iostream>
#include <string>

using namespace asl::containers;

int main() {
    string hello("hello");
    hello.push_back('H');
    std::cout << std::string(hello.begin(), hello.end()) << std::endl;
    return 0;
}
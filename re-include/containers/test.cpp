#include "./string.hpp"
#include <iostream>

using namespace asl::containers;

int main() {
    string hello("Hello world!");
    std::cout << string(hello.begin(), hello.end()).size();
}
#include "./string.hpp"
#include <iostream>

using namespace asl::containers;

int main() {
    string hello("hello");
    std::cout << string(hello.rbegin(), hello.rend()).data();
}
#include "containers/string.hpp"
#include "containers/vector.hpp"
#include "value_wrappers/nullable.hpp"
#include <iostream>

using namespace asl::containers;
using namespace asl::value_wrappers;

void printer(const string& str) {
    std::cout << str.c_data() << std::endl;
}

int main() {
    vector<string> hello({"Hello", "Two"});
    hello.insert(hello.begin(), "third element");
    for (const auto& each : hello)
        printer(each);

    return 0;
}
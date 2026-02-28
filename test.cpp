#include "containers/string.hpp"
#include "value_wrappers/nullable.hpp"

using namespace asl::containers;
using namespace asl::value_wrappers;

void printer(const string& str) {
    for (auto each : str)
        putchar(each);
}

int main() {
    
    return 0;
}
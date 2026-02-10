// Where u test things.

#include <iostream>
#include "include/fs/directory.hpp"

int main() {
    // Dir creation
    fs::directory dir(fs::sfs::current_path()); // TOOK 0.5 MS, SLOWWWWWWW
    const auto list = dir.list();
    std::cout << "Passed" << std::endl;
    for (const auto& each : list)
        std::cout << each.string() << std::endl;
    
    return 0;
}
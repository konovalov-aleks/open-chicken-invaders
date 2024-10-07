#pragma once

#include <cstdlib>
#include <iostream>

namespace oci {

template <typename... MessageArgs>
[[noreturn]] void CriticalError(const MessageArgs&... args) {
    std::cerr << "CRITICAL ERROR: ";
    ([&args] {
        std::cerr << args;
    }(), ...);
    std::cerr << std::endl;
    std::abort();
}

} // namespace oci

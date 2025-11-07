#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <chrono>

struct SockAddr {
    std::string ip;
    uint16_t port{};
};

// inline std::uint64_t now_ms() {
//     return std::chrono::duration_cast<std::chrono::milliseconds>(
//         std::chrono::steady_clock::now().time_since_epoch()).count();
// }

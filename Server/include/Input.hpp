#pragma once
#include <cstdint>

struct Input {
    uint8_t inputBits{};      // Bitmask actions
    uint32_t roomId{};        // Target room id
    uint32_t playerId{};      // Player author
    uint32_t seqNum{};        // Sequence number

    uint8_t getInput() {
        return inputBits;
    };
};

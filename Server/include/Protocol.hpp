#pragma once
#include <cstdint>
#include <vector>
#include <cstddef>
#include <optional>
#include <iostream>
#include "Error.hpp"
#include "Snapshot.hpp"
#include "Input.hpp"

#pragma pack(push, 1)
struct Packet_header {
    uint32_t seq;
    uint32_t player_id;
    uint32_t room_id;
    uint8_t opcode;
    uint16_t playload_len;
};
#pragma pack(pop)

struct DecodedPacket {
    Packet_header header;
    enum class Type { Unknown, Join, Input } type{Type::Unknown};
    uint8_t input;
   // Input input; // valid if type == Input
    // Additional fields (playerId, etc.) could go here
};

class Protocol {
private:
    uint32_t nextSeq_ = 1;
public:
    DecodedPacket decodePacket(const std::vector<uint8_t>& buffer) const;
    std::vector<std::vector<uint8_t>> encodeSnapshot(const Snapshot &snap);
    std::vector<uint8_t> encodeAck(uint32_t seq, uint32_t playerId);

    static std::vector<uint8_t> write_u32_be(uint32_t value);
    static uint32_t read_u32_be(const uint8_t* data, std::size_t len);
    static std::vector<uint8_t> write_float_be(float value);
    static float read_float_be(const uint8_t* data, std::size_t len);
};

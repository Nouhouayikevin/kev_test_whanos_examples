#include "../include/Protocol.hpp"
#include <cstring>
#include <arpa/inet.h>

DecodedPacket Protocol::decodePacket(const std::vector<uint8_t>& buffer) const {
    DecodedPacket pkt;
    
    try {
        // Vérifier que le buffer est assez grand pour contenir au moins l'en-tête
        // const size_t min_header_size = 15; // 4+4+1+4+2 octets
        // if (buffer.size() < min_header_size) {
        //     throw Error("too small buffer, received size " + std::to_string(buffer.size()) + ", expected at least " + std::to_string(min_header_size));
        // }
    std::memcpy(&pkt.header, buffer.data(), sizeof(Packet_header));
    
    pkt.header.seq = ntohl(pkt.header.seq);
    pkt.header.player_id = ntohl(pkt.header.player_id);
    pkt.header.room_id = ntohl(pkt.header.room_id);
    pkt.header.playload_len = ntohs(pkt.header.playload_len);
    pkt.header.opcode = pkt.header.opcode;

    size_t expected_size = sizeof(Packet_header) + pkt.header.playload_len;
    if (buffer.size() < expected_size)
        throw Error("Invalid packet size");

    switch (pkt.header.opcode) {
        case 0x01:
            pkt.type = DecodedPacket::Type::Join;
            pkt.input = 0;
            break;
        case 0x02:
            pkt.type = DecodedPacket::Type::Input;
            if (pkt.header.playload_len > 0) {
                std::memcpy(&pkt.input, buffer.data() + sizeof(Packet_header), static_cast<size_t>(pkt.header.playload_len));
            }
            break;
        default:
            throw Error("Unknown packet, opcode: " + std::to_string(pkt.header.opcode));
    }
    } catch (const Error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return pkt;
}

std::vector<std::vector<uint8_t>> Protocol::encodeSnapshot(const Snapshot &snap) {
    std::vector<std::vector<uint8_t>> packets;
    
    // Utilise la méthode serialize() du Snapshot qui encode tout correctement
    std::vector<uint8_t> serialized = snap.serialize();
    
    packets.push_back(std::move(serialized));
    return packets;
}

std::vector<uint8_t> Protocol::encodeAck(uint32_t seq, uint32_t playerId) {
    std::vector<uint8_t> buf;
    buf.push_back(4); // ack type
    auto s = write_u32_be(seq); buf.insert(buf.end(), s.begin(), s.end());
    auto p = write_u32_be(playerId); buf.insert(buf.end(), p.begin(), p.end());
    return buf;
}

std::vector<uint8_t> Protocol::write_u32_be(uint32_t value) {
    std::vector<uint8_t> b(4);
    b[0] = static_cast<uint8_t>((value >> 24) & 0xFF);
    b[1] = static_cast<uint8_t>((value >> 16) & 0xFF);
    b[2] = static_cast<uint8_t>((value >> 8) & 0xFF);
    b[3] = static_cast<uint8_t>(value & 0xFF);
    return b;
}

uint32_t Protocol::read_u32_be(const uint8_t* data, std::size_t len) {
    if (len < 4) return 0;
    return (static_cast<uint32_t>(data[0]) << 24) |
           (static_cast<uint32_t>(data[1]) << 16) |
           (static_cast<uint32_t>(data[2]) << 8)  |
           (static_cast<uint32_t>(data[3]));
}

std::vector<uint8_t> Protocol::write_float_be(float value) {
    static_assert(sizeof(float)==4, "Unexpected float size");
    std::vector<uint8_t> b(4);
    uint32_t tmp;
    std::memcpy(&tmp, &value, 4);
    auto raw = write_u32_be(tmp);
    return raw;
}

float Protocol::read_float_be(const uint8_t* data, std::size_t len) {
    if (len < 4) return 0.f;
    uint32_t tmp = read_u32_be(data, len);
    float f;
    std::memcpy(&f, &tmp, 4);
    return f;
}

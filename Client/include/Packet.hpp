/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** Packet.hpp
*/
#pragma once
#include <cstdint>

struct Packet_Header {
    uint32_t seq;
    uint32_t player_id;
    uint32_t room_id;
    uint8_t opcode;
    uint16_t playload_len;
} __attribute__((packed));

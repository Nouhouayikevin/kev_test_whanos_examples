/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** Client.cpp
*/

#include "../include/Client.hpp"
#include <iostream>
#include "../../Server/include/Snapshot.hpp"


Client::Client(const std::string& ip, uint16_t port)
    : server_ip(ip), server_port(port),
      network(std::make_unique<UDPClientListiner>(ip, port)),
      protocol(std::make_unique<Protocol>())
{
    std::cout << "[Client] Initialized for " << ip << ":" << port << std::endl;
}

void Client::run()
{
    std::cout << "[Client] Connecting to " << server_ip << ":" << server_port << "..." << std::endl;

    if (!network->start()) {
        std::cerr << "[Error] Failed to start network client." << std::endl;
        return;
    }

    Packet_Header join_header{};
    join_header.seq = htonl(get_next_seq());
    join_header.player_id = htonl(0);
    join_header.room_id = htonl(0);
    join_header.opcode = 0x01;
    join_header.playload_len = htons(0);

    auto join_packet = std::make_shared<std::vector<uint8_t>>(sizeof(Packet_Header));
    memcpy(join_packet->data(), &join_header, sizeof(join_header));

    std::cout << "[Client] Sending join request..." << std::endl;
    boost::system::error_code error;
    std::size_t bytes_transferred = 0;
    if (!network->send(join_packet, error, bytes_transferred)) {
        std::cerr << "[Error] Failed to send join request: " << error.message() << std::endl;
        return;
    }
    std::cout << "[Client] Waiting for server response..." << std::endl;
    while (true) {
        if (!network->receive())
            continue;
        auto [header, payload] = network->getNextPacket();
        uint8_t opcode = header.opcode;
        uint32_t seq = ntohl(header.seq);

        switch (opcode) {
            case 0x02: {
                player_id = htonl(join_header.player_id);
                room_id = htonl(join_header.room_id);
                std::cout << "[Client] Connected successfully!"
                          << " Player ID: " << player_id
                          << " | Room ID: " << room_id << std::endl;
                if (!payload.empty()) {
                    std::cout << "[Client] Server message: " << std::string(payload.begin(), payload.end()) << std::endl;
                }
                connected = true;
                break;
            }
            case 0x10: {
                Snapshot snap;
                std::cout << "[Client] Received snapshot (seq=" << seq << ")" << std::endl;
                snap = protocol->decodeSnapshot(payload);
                break;
            }
            default:
                std::cout << "[Client] Unknown opcode: 0x"
                          << std::hex << (int)opcode << std::dec << std::endl;
                break;
        }
    }

    network->stop();
}
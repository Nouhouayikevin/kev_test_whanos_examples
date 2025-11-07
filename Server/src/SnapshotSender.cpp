#include "../include/SnapshotSender.hpp"
#include "../include/SnapshotHelper.hpp"
#include "../../Include/Server/opcode.hpp"
#include <iostream>
#include <cstring>
#include <boost/bind/bind.hpp>
#include <arpa/inet.h>

SnapshotSender::SnapshotSender(boost::asio::ip::udp::socket& socket, uint32_t roomId)
    : socket_(socket), roomId_(roomId), seqCounter_(0) {
}

void SnapshotSender::sendToAll(const std::vector<Client> &clients, const Snapshot &snapshot) {
    if (clients.empty()) {
        return;
    }

    // Encoder le snapshot avec Protocol::encodeSnapshot
    std::vector<std::vector<uint8_t>> encoded_packets = protocol_.encodeSnapshot(snapshot);
    
    if (encoded_packets.empty()) {
        return;
    }
    
    // Prendre le premier paquet (pour l'instant)
    const std::vector<uint8_t>& playload = encoded_packets[0];
    
    // Encoder le header manuellement en network byte order (big-endian)
    std::vector<uint8_t> buffer;
    
    uint32_t seq_val = seqCounter_++;
    uint32_t player_id_val = 0; // 0 = broadcast à tous
    uint32_t room_id_val = roomId_;
    uint8_t opcode_val = 0x03; // Opcode pour les snapshots
    uint16_t payload_len_val = static_cast<uint16_t>(playload.size());
    
    // seq (4 bytes, big-endian)
    buffer.push_back((seq_val >> 24) & 0xFF);
    buffer.push_back((seq_val >> 16) & 0xFF);
    buffer.push_back((seq_val >> 8) & 0xFF);
    buffer.push_back(seq_val & 0xFF);
    
    // player_id (4 bytes, big-endian)
    buffer.push_back((player_id_val >> 24) & 0xFF);
    buffer.push_back((player_id_val >> 16) & 0xFF);
    buffer.push_back((player_id_val >> 8) & 0xFF);
    buffer.push_back(player_id_val & 0xFF);
    
    // room_id (4 bytes, big-endian)
    buffer.push_back((room_id_val >> 24) & 0xFF);
    buffer.push_back((room_id_val >> 16) & 0xFF);
    buffer.push_back((room_id_val >> 8) & 0xFF);
    buffer.push_back(room_id_val & 0xFF);
    
    // opcode (1 byte)
    buffer.push_back(opcode_val);
    
    // payload_len (2 bytes, big-endian)
    buffer.push_back((payload_len_val >> 8) & 0xFF);
    buffer.push_back(payload_len_val & 0xFF);
    
    // Ajouter le payload
    buffer.insert(buffer.end(), playload.begin(), playload.end());

    // Envoyer à chaque client
    for (const auto &client : clients) {
        try {
            // Créer un shared_ptr pour le buffer (nécessaire pour async_send_to)
            auto buffer_ptr = std::make_shared<std::vector<uint8_t>>(buffer);
            
            // Envoi asynchrone comme dans UDPListener
            socket_.async_send_to(
                boost::asio::buffer(*buffer_ptr),
                client.getEndpoint(),
                [buffer_ptr](const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
                    if (error) {
                        std::cerr << "Error sending snapshot: " << error.message() << std::endl;
                    }
                }
            );
        } catch (const std::exception& e) {
            std::cerr << "Exception sending snapshot to client: " << e.what() << std::endl;
        }
    }
}

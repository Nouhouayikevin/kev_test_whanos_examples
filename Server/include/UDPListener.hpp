#pragma once
#include <cstdint>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "Types.hpp"
#include "Protocol.hpp"
#include "Input.hpp"
#include "Client.hpp"
#include "Error.hpp"
#include "RoomManager.hpp"
#include "../../Include/GameEngine_Include/core/GameEngine.hpp"


class UDPListener : public INetwork {
public:
    using udp = boost::asio::ip::udp;
    using endpoint = udp::endpoint;
    
    UDPListener(boost::asio::io_context& io_context, uint16_t port);
    ~UDPListener();

    bool start() override;
    bool stop() override;
    
    // bool hasNewInput() const { return has_new_input_; }
    // bool hasNewClient() const { return has_new_client_; }
    
    Input getInput();
    Client getClient();
    bool isRunning() const { return running_; }

private:
    std::pair<Packet_Header, std::vector<uint8_t>> getNextPacket() override{
        throw std::runtime_error("Not implemented");
    };
    bool receive() override;
    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);
    bool send(std::shared_ptr<std::vector<uint8_t>> message, const boost::system::error_code& error, std::size_t bytes_transferred) override;
    
    void handle_packet(const std::vector<uint8_t>& buffer, const endpoint& sender_endpoint);
    void handle_join_request(const DecodedPacket& packet, const endpoint& endpoint);
    void handle_input(const DecodedPacket& packet, const endpoint& endpoint);
    
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    std::array<uint8_t, 65536> recv_buffer_;

    Protocol protocol_;
    std::atomic<bool> running_{false};

    std::mutex mutex_;
    
    Input client_input_;
    Client new_client_;
    // bool has_new_input_{false};
    // bool has_new_client_{false};
    RoomManager roomManager_;

    
    std::unordered_map<std::string, Client> clients_;
    std::atomic<uint32_t> id_count_{0};
};

/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** NetworkClient.hpp
*/

#ifndef INCLUDED_UDPCLIENTLISTINER_HPP
#define INCLUDED_UDPCLIENTLISTINER_HPP

#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <memory>
#include "Packet.hpp"
#include "../../Include/All/interfaces/INetwork.hpp"

class UDPClientListiner : public INetwork {
public:
    UDPClientListiner(const std::string& server_ip, uint16_t server_port);
    ~UDPClientListiner();

    bool start() override;
    bool stop() override;
    bool receive() override;
    bool send(std::shared_ptr<std::vector<uint8_t>> message, const boost::system::error_code& error, std::size_t bytes_transferred) override;
    std::pair<Packet_Header, std::vector<uint8_t>> getNextPacket() override;
    bool isRunning() const override { return running_.load(); }

private:
    void start_receive();

    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::ip::udp::socket> socket_;
    boost::asio::ip::udp::endpoint server_endpoint_;
    std::thread io_thread_;
    std::queue<std::pair<Packet_Header, std::vector<uint8_t>>> received_packets_;
    std::mutex queue_mutex_;
    std::atomic<bool> running_{false};
};
#endif
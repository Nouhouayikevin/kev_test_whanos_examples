/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sat, Oct, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-24                                         *
 * Description     -                                                                    *
 *     UDPClientListiner                                                                    *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _|           *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _|           *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_|           *
 *       _|        _|          _|        _|      _|        _|        _|    _|           *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _|           *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "../include/NetworkClient.hpp"
#include <iostream>
#include <boost/system/error_code.hpp>

UDPClientListiner::UDPClientListiner(const std::string& server_ip, uint16_t server_port)
    : socket_(std::make_unique<boost::asio::ip::udp::socket>(io_context_))
    , server_endpoint_(boost::asio::ip::make_address(server_ip), server_port)
    , running_(false)
{
    std::cout << "UDP Client initialized for " << server_ip << ":" << server_port << std::endl;
}

UDPClientListiner::~UDPClientListiner()
{
    stop();
}

bool UDPClientListiner::start()
{
    try {
        socket_->open(boost::asio::ip::udp::v4());
        socket_->connect(server_endpoint_);
        running_ = true;

        start_receive();
        io_thread_ = std::thread([this]() { io_context_.run(); });

        std::cout << "[Client] Connection to server established." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[Error] Failed to start client: " << e.what() << std::endl;
        return false;
    }
}

bool UDPClientListiner::stop()
{
    if (!running_)
        return true;
    running_ = false;

    boost::system::error_code ec;
    if (socket_ && socket_->is_open()) {
        socket_->close(ec);
        if (ec)
            std::cerr << "[Error] Failed to close socket: " << ec.message() << std::endl;
    }
    io_context_.stop();

    if (io_thread_.joinable())
        io_thread_.join();
    std::cout << "[Client] Connection closed." << std::endl;
    return true;
}

void UDPClientListiner::start_receive() {
    if (!running_)
        return;

    auto buffer = std::make_shared<std::vector<uint8_t>>(1500);
    socket_->async_receive(boost::asio::buffer(*buffer),
        [this, buffer](const boost::system::error_code& ec, std::size_t len) {
            if (!ec && len >= 15) {  // 15 = taille minimale du header
                // Décoder manuellement le header (network byte order = big-endian)
                Packet_Header header{};
                const uint8_t* data = buffer->data();
                
                // seq (4 bytes)
                header.seq = (static_cast<uint32_t>(data[0]) << 24) |
                             (static_cast<uint32_t>(data[1]) << 16) |
                             (static_cast<uint32_t>(data[2]) << 8) |
                             static_cast<uint32_t>(data[3]);
                
                // player_id (4 bytes)
                header.player_id = (static_cast<uint32_t>(data[4]) << 24) |
                                   (static_cast<uint32_t>(data[5]) << 16) |
                                   (static_cast<uint32_t>(data[6]) << 8) |
                                   static_cast<uint32_t>(data[7]);
                
                // room_id (4 bytes)
                header.room_id = (static_cast<uint32_t>(data[8]) << 24) |
                                 (static_cast<uint32_t>(data[9]) << 16) |
                                 (static_cast<uint32_t>(data[10]) << 8) |
                                 static_cast<uint32_t>(data[11]);
                
                // opcode (1 byte)
                header.opcode = data[12];
                
                // playload_len (2 bytes)
                header.playload_len = (static_cast<uint16_t>(data[13]) << 8) |
                                      static_cast<uint16_t>(data[14]);

                std::vector<uint8_t> payload;
                if (header.playload_len > 0 && len >= 15 + header.playload_len) {
                    payload.assign(buffer->begin() + 15,
                                buffer->begin() + 15 + header.playload_len);
                }

                std::lock_guard<std::mutex> lock(queue_mutex_);
                received_packets_.push({header, payload});
            }
            if (running_)
                start_receive();
        });
}

bool UDPClientListiner::receive()
{
    return !received_packets_.empty();
}

bool UDPClientListiner::send(std::shared_ptr<std::vector<uint8_t>> message, const boost::system::error_code& error, std::size_t bytes_transferred) {
    try {
        if (!message || message->empty()) {
            std::cerr << "[Error] Empty or null message provided to send." << std::endl;
            const_cast<boost::system::error_code&>(error) = boost::system::error_code(
                boost::system::errc::invalid_argument, boost::system::system_category());
            const_cast<std::size_t&>(bytes_transferred) = 0;
            return false;
        }
        const_cast<std::size_t&>(bytes_transferred) = socket_->send(boost::asio::buffer(*message));
        const_cast<boost::system::error_code&>(error) = boost::system::error_code();
        return true;
    } catch (const std::exception& e) {
        const_cast<boost::system::error_code&>(error) = boost::system::error_code(
            boost::system::errc::io_error, boost::system::system_category());
        const_cast<std::size_t&>(bytes_transferred) = 0;
        std::cerr << "[Error] Failed to send data: " << e.what() << std::endl;
        return false;
    }
}

std::pair<Packet_Header, std::vector<uint8_t>> UDPClientListiner::getNextPacket()
{
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (received_packets_.empty())
        return {{0, 0, 0, 0, 0}, {}};
    auto packet = received_packets_.front();
    received_packets_.pop();
    return packet;
}
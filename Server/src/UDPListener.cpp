#include "../include/UDPListener.hpp"
#include <iostream>
#include <boost/bind/bind.hpp>

using namespace boost::placeholders;

UDPListener::UDPListener(boost::asio::io_context& io_context, uint16_t port)
    : socket_(io_context, udp::endpoint(udp::v4(), port))
    , running_(false)
    , roomManager_(socket_)
{
    std::cout << "UDP Server started on port " << port << std::endl;
}

UDPListener::~UDPListener()
{
    stop();
}

bool UDPListener::start()
{
    std::cout << "UDPListener starting..." << std::endl;
    if (running_) return false;
    running_ = true;
    receive();
    return true;
}

bool UDPListener::stop()
{
    if (!running_) return false;
    running_ = false;
    
    boost::system::error_code ec;
    socket_.close(ec);
    if (ec) {
        std::cerr << "Error closing socket: " << ec.message() << std::endl;
    }
    return true;
}

bool UDPListener::receive()
{
    if (!running_) return false;
    std::cout << "Waiting for data..." << std::endl;
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&UDPListener::handle_receive, this,
                   boost::asio::placeholders::error,
                   boost::asio::placeholders::bytes_transferred));
    return true;
}

void UDPListener::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error && bytes_transferred > 0) {
        try {
            std::vector<uint8_t> buffer(recv_buffer_.begin(), recv_buffer_.begin() + bytes_transferred);
            handle_packet(buffer, remote_endpoint_);
        } catch (const std::exception& e) {
            std::cerr << "Error handling packet: " << e.what() << std::endl;
        }
    } else if (error) {
        std::cerr << "Receive error: " << error.message() << std::endl;
    }
    if (running_) {
        receive();
    }
}

void UDPListener::handle_packet(const std::vector<uint8_t>& buffer, const endpoint& sender_endpoint)
{
    try {
        auto packet = protocol_.decodePacket(buffer);
        std::lock_guard<std::mutex> lock(mutex_);
        
        switch (packet.type) {
            case DecodedPacket::Type::Join:
                handle_join_request(packet, sender_endpoint);
                break;
            case DecodedPacket::Type::Input:
                handle_input(packet, sender_endpoint);
                break;
            default:
                std::cerr << "Unknown packet type received" << std::endl;
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error decoding packet: " << e.what() << std::endl;
    }
}

bool UDPListener::send(std::shared_ptr<std::vector<uint8_t>> message, const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (error) {
        std::cerr << "Error sending data: " << error.message() << std::endl;
        return false;
    }
    if (bytes_transferred != message->size()) {
        std::cerr << "Error sending data: incomplete transfer" << std::endl;
        return false;
    }
    return true;
}

void UDPListener::handle_join_request(const DecodedPacket& packet, const endpoint& endpoint)
{
    std::string client_id = endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
    
    //check if client already exists
    auto it = clients_.find(client_id);
    if (it == clients_.end()) {
        //new client
        id_count_++;
        new_client_ = Client(id_count_, endpoint.address().to_string());
        new_client_.setEndpoint(endpoint);  // Définir l'endpoint pour l'envoi des snapshots
        clients_[client_id] = new_client_;
        // has_new_client_ = true;
        
        roomManager_.assignClientToRoom(new_client_);
        
        // Create join response - encoder manuellement en network byte order
        std::vector<uint8_t> buffer;
        
        uint32_t seq_val = packet.header.seq;
        uint32_t player_id_val = id_count_;
        uint32_t room_id_val = new_client_.getRoomId();
        uint16_t payload_len = 2; // "OK"
        
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
        buffer.push_back(0x02);
        
        // payload_len (2 bytes, big-endian)
        buffer.push_back((payload_len >> 8) & 0xFF);
        buffer.push_back(payload_len & 0xFF);
        
        // payload
        std::string response = "OK";
        buffer.insert(buffer.end(), response.begin(), response.end());

        // Send join response
        auto rsp_ptr = std::make_shared<std::vector<uint8_t>>(buffer);
        socket_.async_send_to(
            boost::asio::buffer(*rsp_ptr), endpoint,
            boost::bind(&UDPListener::send, this,
                       rsp_ptr,
                       boost::asio::placeholders::error,
                       boost::asio::placeholders::bytes_transferred));
    }
}

void UDPListener::handle_input(const DecodedPacket& packet, const endpoint& endpoint)
{
    std::string client_id = endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
    auto it = clients_.find(client_id);
    
    if (it != clients_.end()) {
        client_input_.inputBits = packet.input;
        client_input_.roomId = packet.header.room_id;
        client_input_.playerId = packet.header.player_id;
        client_input_.seqNum = packet.header.seq;
        
        roomManager_.assign_Input_to_Room(client_input_);

        // Encoder l'ACK manuellement en network byte order
        std::vector<uint8_t> ack_buffer;
        std::string response = "OK";
        
        uint32_t seq_val = packet.header.seq;
        uint32_t player_id_val = packet.header.player_id;
        uint32_t room_id_val = packet.header.room_id;
        uint16_t payload_len = response.size();
        
        // seq (4 bytes, big-endian)
        ack_buffer.push_back((seq_val >> 24) & 0xFF);
        ack_buffer.push_back((seq_val >> 16) & 0xFF);
        ack_buffer.push_back((seq_val >> 8) & 0xFF);
        ack_buffer.push_back(seq_val & 0xFF);
        
        // player_id (4 bytes, big-endian)
        ack_buffer.push_back((player_id_val >> 24) & 0xFF);
        ack_buffer.push_back((player_id_val >> 16) & 0xFF);
        ack_buffer.push_back((player_id_val >> 8) & 0xFF);
        ack_buffer.push_back(player_id_val & 0xFF);
        
        // room_id (4 bytes, big-endian)
        ack_buffer.push_back((room_id_val >> 24) & 0xFF);
        ack_buffer.push_back((room_id_val >> 16) & 0xFF);
        ack_buffer.push_back((room_id_val >> 8) & 0xFF);
        ack_buffer.push_back(room_id_val & 0xFF);
        
        // opcode (1 byte)
        ack_buffer.push_back(0x03);
        
        // payload_len (2 bytes, big-endian)
        ack_buffer.push_back((payload_len >> 8) & 0xFF);
        ack_buffer.push_back(payload_len & 0xFF);
        
        // payload
        ack_buffer.insert(ack_buffer.end(), response.begin(), response.end());

        // Send input ACK
        auto rsp_ptr = std::make_shared<std::vector<uint8_t>>(ack_buffer);
        socket_.async_send_to(
            boost::asio::buffer(*rsp_ptr), endpoint,
            boost::bind(&UDPListener::send, this,
                       rsp_ptr,
                       boost::asio::placeholders::error,
                       boost::asio::placeholders::bytes_transferred));

    } else {
        std::cerr << "Received input from unknown client: " << client_id << std::endl;
    }
}

Input UDPListener::getInput()
{
    // std::lock_guard<std::mutex> lock(mutex_);
    // has_new_input_ = false;
    return client_input_;
}

Client UDPListener::getClient()
{
    // std::lock_guard<std::mutex> lock(mutex_);
    // has_new_client_ = false;
    return new_client_;
}
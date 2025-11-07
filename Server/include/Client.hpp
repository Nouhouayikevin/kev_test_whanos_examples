#pragma once
#include <cstdint>
#include <vector>
#include "Types.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <boost/asio.hpp>

class Client {
private:
    uint32_t id_{};
    std::string addr_;
    uint32_t lastSeqReceived{};
    std::uint64_t lastSeenMs{};
    uint32_t room_id;
    boost::asio::ip::udp::endpoint endpoint_;
    
public:
    Client() : id_(0), addr_(""), lastSeqReceived(0), lastSeenMs(0) {}
    Client (uint32_t id, std::string addr);
    void setId(uint32_t id);
    uint32_t getId(void) const;
    void setAddr(std::string addr);
    void clear();
    void setRoomId(uint32_t roomId);
    uint32_t getRoomId(void) const;
    bool isEmpty(void);
    
    // Endpoint management
    void setEndpoint(const boost::asio::ip::udp::endpoint& endpoint) { endpoint_ = endpoint; }
    const boost::asio::ip::udp::endpoint& getEndpoint() const { return endpoint_; }
    
    ~Client();
};


// NOTE: processNetworkEvents() is commented out because RoomManager is not used in ServerApplication
// This functionality is handled by the individual Room threads
/*
void ServerApplication::processNetworkEvents()
{
    
    //check for new clients
    auto& udp_listener = static_cast<UDPListener&>(gameEngine_.getNetworkManager());
    if (udp_listener.hasNewClient()) {
        Client newClient = udp_listener.getClient();
        std::cout << "New client connected: " << newClient.getId() << std::endl;
        roomManager_.assignClientToRoom(newClient);
    }
    
    //process input from clients
    if (udp_listener.hasNewInput()) {
        Input input = udp_listener.getInput();
        if (input.playerId != 0) {
            std::cout << "New input received: " << input.playerId << std::endl;
            roomManager_.assign_Input_to_Room(input);
        }
    }
}
*/

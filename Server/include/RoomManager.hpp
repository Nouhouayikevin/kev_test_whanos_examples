#pragma once
#include <map>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>
#include <boost/asio.hpp>

// Forward declarations
class Room;
class Input;
class Client;

class RoomManager {
private:
    std::vector<std::thread> threads_;
    std::map<int, std::shared_ptr<Room>> rooms_;
    int nextRoomId_ = 1;
    mutable std::mutex roomsMutex_;
    boost::asio::ip::udp::socket& socket_;
    
public:
    RoomManager(boost::asio::ip::udp::socket& socket) : socket_(socket) {}
    
    std::shared_ptr<Room> createRoom();
    void assign_Input_to_Room(const Input &input);
    void assignClientToRoom(Client &client);
    std::shared_ptr<Room> getRoom(int id);
    void startRoom(int id);
    void stopRoom(int id);
    ~RoomManager();
};

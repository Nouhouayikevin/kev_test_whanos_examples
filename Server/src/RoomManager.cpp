#include "../include/RoomManager.hpp"
#include "../include/Room.hpp"
#include "../include/Client.hpp"
#include "../include/Input.hpp"

#include <algorithm>

std::shared_ptr<Room> RoomManager::createRoom() {
    auto room = std::make_shared<Room>(nextRoomId_++, socket_);
    {
        std::lock_guard<std::mutex> lock(roomsMutex_);
        rooms_[room->id()] = room;
    }
    threads_.emplace_back([room]{ room->run(); });
    return room;
}

void RoomManager::assignClientToRoom(Client &client) {
    std::shared_ptr<Room> target;
    {
        std::lock_guard<std::mutex> lock(roomsMutex_);
        for (auto &kv : rooms_) {
            if (!kv.second->isFull()) { target = kv.second; break; }
        }
    }
    if (!target) { target = createRoom(); }
    client.setRoomId(target->id());
    target->addClient(client);
}

void RoomManager::assign_Input_to_Room(const Input &input) {
    std::shared_ptr<Room> r;
    {
        std::lock_guard<std::mutex> lock(roomsMutex_);
        auto it = rooms_.find(static_cast<int>(input.roomId));
        if (it == rooms_.end()) return;
        r = it->second;
    }
    r->queue().push(input);
}

std::shared_ptr<Room> RoomManager::getRoom(int id) {
    std::lock_guard<std::mutex> lock(roomsMutex_);
    auto it = rooms_.find(id);
    if (it != rooms_.end()) return it->second;
    return nullptr;
}

void RoomManager::startRoom(int id) {
    std::shared_ptr<Room> r;
    {
        std::lock_guard<std::mutex> lock(roomsMutex_);
        auto it = rooms_.find(id);
        if (it == rooms_.end()) return;
        r = it->second;
    }
    threads_.emplace_back([r]{ r->run(); });
}

void RoomManager::stopRoom(int id) {
    std::shared_ptr<Room> r;
    {
        std::lock_guard<std::mutex> lock(roomsMutex_);
        auto it = rooms_.find(id);
        if (it == rooms_.end()) return;
        r = it->second;
    }
    r->stop();
}

RoomManager::~RoomManager() {
    {
        std::lock_guard<std::mutex> lock(roomsMutex_);
        for (auto &kv : rooms_) kv.second->stop();
    }
    for (auto &t : threads_) if (t.joinable()) t.join();
}

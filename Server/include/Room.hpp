#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <boost/asio.hpp>

#include "Client.hpp"
#include "InputQueue.hpp"
#include "GameLogic.hpp"
#include "SnapshotSender.hpp"
#include "../../Include/All/managers/SceneManager.hpp"
#include "../../Include/All/Scenes/Online_GameScene_server.hpp"
#include "ServerApplication.hpp"

class Room {
private:
    uint32_t roomId_{};
    std::vector<Client> clients_;
    InputQueue inputQueue_;
    GameEngine gameSceneEngine_;
    OnlineGameScene_server game {inputQueue_};
    ServerApplication serverApp_;
    SnapshotSender snapshotSender_;
    bool running_{false};
    bool gameSceneInitialized_{false};
    uint32_t tickMs_ = 16; // ~60fps
    std::size_t kMaxClients = 4;
    uint32_t tickCounter_ = 0;
    std::mutex clientsMutex_;


public:
    Room(uint32_t id, boost::asio::ip::udp::socket& socket) 
        : roomId_(id), snapshotSender_(socket, id) {}
    ~Room() = default;

    void run();
    void stop() { running_ = false; }
    void addClient(Client &client);
    void broadcast(const Snapshot &snapshot);
    void processInputs();

    bool isFull() const { return clients_.size() >= kMaxClients; }
    uint32_t id() const { return roomId_; }
    InputQueue &queue() { return inputQueue_; }
};

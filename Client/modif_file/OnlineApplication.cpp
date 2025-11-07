/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-24                                         *
 * Description     -                                                                    *
 *     OnlineApplication                                                                *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *         ░        ░       ░░        ░        ░        ░░      ░░  ░░░░  ░             *
 *         ▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒▒▒▒▒▒▒  ▒▒▒▒  ▒  ▒▒▒▒  ▒             *
 *         ▓      ▓▓▓       ▓▓▓▓▓  ▓▓▓▓▓▓▓  ▓▓▓▓      ▓▓▓  ▓▓▓▓▓▓▓        ▓             *
 *         █  ███████  ██████████  ███████  ████  ███████  ████  █  ████  █             *
 *         █        █  ███████        ████  ████        ██      ██  ████  █             *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

#include "../include/OnlineApplication.hpp"
#include "../../Include/All/sfml/SfmlWindow.hpp"
#include "../../Include/All/sfml/SfmlInputManager.hpp"
#include "../../Include/All/sfml/SfmlResourceFactory.hpp"
#include <bitset>
#include "../../Include/All/Scenes/OnlineClientGameScene.hpp"
#include "../include/Protocole.hpp"
#include "../../Server/include/Snapshot.hpp"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>

namespace ProtocolInputs {
    const uint8_t OPCODE_INPUT = 0x02;

    namespace Input {
        const uint8_t UP    = 1 << 0; // 0b00000001
        const uint8_t DOWN  = 1 << 1; // 0b00000010
        const uint8_t LEFT  = 1 << 2; // 0b00000100
        const uint8_t RIGHT = 1 << 3; // 0b00001000
        const uint8_t SHOOT = 1 << 4; // 0b00010000
    }
}

OnlineApplication::OnlineApplication(const std::string& ip, uint16_t port)
    : server_ip(ip), server_port(port), _input_send_timer(0.0f),
      protocol(std::make_unique<Protocol>()) {
    std::cout << "[OnlineApplication] Initialized for " << ip << ":" << port << std::endl;
}

OnlineApplication::~OnlineApplication()
{
    shutdown();
}

void OnlineApplication::initialize()
{
    std::cout << "Initializing OnlineApplication..." << std::endl;
    auto network_module = std::make_unique<UDPClientListiner>(server_ip, server_port);
    engine->setNetwork(std::move(network_module));


    engine->getSceneManager().add_scene("online_game", std::make_shared<ClientOnlineGameScene>());
    engine->getSceneManager().switch_to_scene("online_game", *engine);

    if (!engine->getNetworkManager().start()) {
        std::cerr << "[Error] Failed to start network." << std::endl;
        throw std::runtime_error("Network initialization failed");
    }

    std::cout << "OnlineApplication initialized successfully." << std::endl;
}

void OnlineApplication::handle_network()
{
    INetwork& network_manager = engine->getNetworkManager();
    Packet_Header join_header{};
    join_header.seq = htonl(seq++);
    join_header.player_id = htonl(0);
    join_header.room_id = htonl(0);
    join_header.opcode = 0x01;
    join_header.playload_len = htons(0);

    auto join_packet = std::make_shared<std::vector<uint8_t>>(sizeof(Packet_Header));
    memcpy(join_packet->data(), &join_header, sizeof(join_header));

    boost::system::error_code error;
    std::size_t bytes_transferred = 0;
    if (!network_manager.send(join_packet, error, bytes_transferred)) {
        std::cerr << "[Error] Failed to send join request: " << error.message() << std::endl;
        return;
    }
    std::cout << "[OnlineApplication] Sent join request." << std::endl;

    while (!stop_requested && network_manager.isRunning()) {
        if (network_manager.receive()) {
            auto [header, payload] = network_manager.getNextPacket();
            uint8_t opcode = header.opcode;
            uint32_t seq = header.seq;  // Déjà décodé par NetworkClient

            switch (opcode) {
                case 0x02: // join response
                    player_id = header.player_id;  // Déjà décodé par NetworkClient
                    room_id = static_cast<uint16_t>(header.room_id);  // Déjà décodé
                    std::cout << "[OnlineApplication] Connected! Player ID: " << player_id
                              << " | Room ID: " << room_id << std::endl;
                    connected = true;
                    break;
                case 0x03: { // snapshot reponse
                    if (payload.empty()) {
                        break;
                    }
                    Snapshot snap = protocol->decodeSnapshot(payload);
                    std::lock_guard<std::mutex> lock(_queue_mutex);
                    _snapshot_queue.push(snap);
                    break;
                }
                default:
                    std::cout << "[OnlineApplication] Unknown opcode: 0x"
                              << std::hex << (int)opcode << std::dec << std::endl;
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void OnlineApplication::run()
{
    std::cout << "Starting online game loop..." << std::endl;
    stop_requested = false;
    network_thread = std::thread(&OnlineApplication::handle_network, this);

    Clock& clock = engine->getClock();
    IWindow& window = engine->getWindowManager();
    IInputManager& input = engine->getInputManager();

    while (!stop_requested && window.isOpen()) {
        float dt = clock.restart();
        input.update();
        if (input.isWindowClosed())
            window.close();

        _input_send_timer += dt;
        if (connected && _input_send_timer >= INPUT_SEND_INTERVAL) {
            _input_send_timer -= INPUT_SEND_INTERVAL;
            uint8_t current_input_mask = 0;
            if (input.isKeyDown(KeyCode::Up))
                current_input_mask |= ProtocolInputs::Input::UP;
            if (input.isKeyDown(KeyCode::Down))
                current_input_mask |= ProtocolInputs::Input::DOWN;
            if (input.isKeyDown(KeyCode::Left))
                current_input_mask |= ProtocolInputs::Input::LEFT;
            if (input.isKeyDown(KeyCode::Right))
                current_input_mask |= ProtocolInputs::Input::RIGHT;
            if (input.isKeyDown(KeyCode::Space))
                current_input_mask |= ProtocolInputs::Input::SHOOT;

            if (current_input_mask != _last_sent_input_mask) {
                _last_sent_input_mask = current_input_mask;
                
                // Encoder le header manuellement en network byte order (big-endian)
                auto packet_data = std::make_shared<std::vector<uint8_t>>();
                
                uint32_t current_seq = seq++;
                
                // seq (4 bytes, big-endian) - écriture manuelle
                packet_data->push_back((current_seq >> 24) & 0xFF);
                packet_data->push_back((current_seq >> 16) & 0xFF);
                packet_data->push_back((current_seq >> 8) & 0xFF);
                packet_data->push_back(current_seq & 0xFF);
                
                // player_id (4 bytes, big-endian)
                packet_data->push_back((player_id >> 24) & 0xFF);
                packet_data->push_back((player_id >> 16) & 0xFF);
                packet_data->push_back((player_id >> 8) & 0xFF);
                packet_data->push_back(player_id & 0xFF);
                
                // room_id (4 bytes, big-endian)
                uint32_t room_id = 1;
                packet_data->push_back((room_id >> 24) & 0xFF);
                packet_data->push_back((room_id >> 16) & 0xFF);
                packet_data->push_back((room_id >> 8) & 0xFF);
                packet_data->push_back(room_id & 0xFF);
                
                // opcode (1 byte)
                packet_data->push_back(ProtocolInputs::OPCODE_INPUT);
                
                // playload_len (2 bytes, big-endian)
                uint16_t payload_len = sizeof(current_input_mask);
                packet_data->push_back((payload_len >> 8) & 0xFF);
                packet_data->push_back(payload_len & 0xFF);
                
                // payload (input mask)
                packet_data->push_back(current_input_mask);

                boost::system::error_code error;
                std::size_t bytes_transferred = 0;
                if (!engine->getNetworkManager().send(packet_data, error, bytes_transferred)) {
                    std::cerr << "[Error] Failed to send input packet: " << error.message() << std::endl;
                }
            }
        }
        std::shared_ptr<IScene> current_scene_ptr = engine->getSceneManager().get_current_scene();

        std::shared_ptr<ClientOnlineGameScene> game_scene = std::dynamic_pointer_cast<ClientOnlineGameScene>(current_scene_ptr);
        if (game_scene) {
            std::lock_guard<std::mutex> lock(_queue_mutex);
            while (!_snapshot_queue.empty()) {
                game_scene->queueSnapshot(_snapshot_queue.front());
                _snapshot_queue.pop();
            }
        }
        engine->getSceneManager().update_current(*engine, dt);
        window.clear();
        engine->getSceneManager().render_current(*engine);
        window.display();
    }
    std::cout << "Online game loop finished." << std::endl;
}

void OnlineApplication::shutdown()
{
    std::cout << "Shutting down OnlineApplication..." << std::endl;
    stop_requested = true;
    try {
        if (engine)
            engine->getNetworkManager().stop();
    } catch (const std::runtime_error& e) {
        std::cerr << "Could not get network manager during shutdown: " << e.what() << std::endl;
    }
    if (network_thread.joinable())
        network_thread.join();
    //engine->shutdown();
    std::cout << "OnlineApplication shutdown complete." << std::endl;
}
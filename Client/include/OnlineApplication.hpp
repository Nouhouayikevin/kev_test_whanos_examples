/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Wed, Oct, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-24                                         *
 * Description     -                                                                    *
 *     OnlineApplication                                                                *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ *
 *                                                                                      *
 *       ▄▀▀█▄▄▄▄  ▄▀▀▄▀▀▀▄  ▄▀▀█▀▄    ▄▀▀▀█▀▀▄  ▄▀▀█▄▄▄▄  ▄▀▄▄▄▄   ▄▀▀▄ ▄▄             *
 *      ▐  ▄▀   ▐ █   █   █ █   █  █  █    █  ▐ ▐  ▄▀   ▐ █ █    ▌ █  █   ▄▀            *
 *        █▄▄▄▄▄  ▐  █▀▀▀▀  ▐   █  ▐  ▐   █       █▄▄▄▄▄  ▐ █      ▐  █▄▄▄█             *
 *        █    ▌     █          █        █        █    ▌    █         █   █             *
 *       ▄▀▄▄▄▄    ▄▀        ▄▀▀▀▀▀▄   ▄▀        ▄▀▄▄▄▄    ▄▀▄▄▄▄▀   ▄▀  ▄▀             *
 *       █    ▐   █         █       █ █          █    ▐   █     ▐   █   █               *
 *       ▐        ▐         ▐       ▐ ▐          ▐        ▐         ▐   ▐               *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

 #ifndef ONLINEAPPLICATION_HPP
#define ONLINEAPPLICATION_HPP

#include "../All/interfaces/IApplication.hpp"
#include "../GameEngine_Include/core/GameEngine.hpp"
#include "../All/managers/SceneManager.hpp"
#include "../include/NetworkClient.hpp"
#include "../include/Protocole.hpp"
#include "../../Server/include/Snapshot.hpp"
#include <memory>
#include <thread>
#include <atomic>

class OnlineApplication : public IApplication {
    public:
        OnlineApplication(const std::string& ip, uint16_t port);
        ~OnlineApplication();

        void initialize() override;
        void run() override;
        void shutdown() override;
        void setEngine(std::shared_ptr<GameEngine> game_engine) {
            engine = game_engine;
        }

        uint32_t get_player_id() const { return player_id; }
        uint16_t get_room_id() const { return room_id; }

    private:
        void handle_network();

        std::shared_ptr<GameEngine> engine;
        std::unique_ptr<Protocol> protocol;
        std::string server_ip;
        uint16_t server_port;
        uint32_t player_id = 0;
        uint16_t room_id = 1;
        uint32_t seq = 0;
        bool connected = false;
        std::thread network_thread;
        std::atomic<bool> stop_requested{false};

        std::queue<Snapshot> _snapshot_queue;
        std::mutex _queue_mutex;
        float _input_send_timer;
        uint8_t _last_sent_input_mask = 0;
        static constexpr float INPUT_SEND_INTERVAL = 1.0f / 20.0f;
};

#endif

/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * MIT License                                                                          *
 * Copyright (c) 2025 Anonymous                                                         *
 *                                                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a copy         *
 * of this software and associated documentation files (the "Software"), to deal        *
 * in the Software without restriction, including without limitation the rights         *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell            *
 * copies of the Software, and to permit persons to whom the Software is                *
 * furnished to do so, subject to the following conditions:                             *
 *                                                                                      *
 * The above copyright notice and this permission notice shall be included in all       *
 * copies or substantial portions of the Software.                                      *
 *                                                                                      *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR           *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,             *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE          *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER               *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,        *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE        *
 * SOFTWARE.                                                                            *
 *                                                                                      *
 * ------------------------------------------------------------------------------------ */

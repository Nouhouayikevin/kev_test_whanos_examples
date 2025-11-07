/* ------------------------------------------------------------------------------------ *
 *                                                                                      *
 * EPITECH PROJECT - Sun, Sep, 2025                                                     *
 * Title           - G-CPP-500-COT-5-1-rtype-24                                         *
 * Description     -                                                                    *
 *     C2S_PLAYER_INPUT                                                                 *
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

#ifndef INCLUDED_C2S_PLAYER_INPUT_HPP
    #define INCLUDED_C2S_PLAYER_INPUT_HPP

#include "opcode.hpp"
#include <cstdint>
#include <vector>

class C2S_PLAYER_INPUT {
    
    public:
        class Error : public std::exception {
            public:
                explicit Error(const char* msg) : _msg(msg) {}
                const char* what() const noexcept override {
                    return _msg;
                }
            private:
                const char* _msg;
        };


        C2S_PLAYER_INPUT(uint8_t input_bits = 0) : input(input_bits) {}
        ~C2S_PLAYER_INPUT() = default;

        uint8_t get_input() const {
            return input;
        }
        void set_input(uint8_t input_bits) {
            input = input_bits;
        }
        std::vector<uint8_t> serialize() const {
            return {static_cast<uint8_t>(Opcode::C2S_PLAYER_INPUT), input};
        }
        static C2S_PLAYER_INPUT deserialize(const std::vector<uint8_t> &buffer) {
            if (buffer.size() != 2 || buffer[0] != static_cast<uint8_t>(Opcode::C2S_PLAYER_INPUT)) {
                throw(Error("Invalid C2S_PLAYER_INPUT packet"));
            }
            return C2S_PLAYER_INPUT(buffer[1]);
        }

    private:
        uint8_t input;
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

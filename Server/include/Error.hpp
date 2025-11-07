/*
** EPITECH PROJECT, 2025
** G-CPP-500-COT-5-1-rtype-24
** File description:
** Error.hpp
*/

#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>

class Error {
    private:
    std::string message;
    public:
    Error(const std::string& _message) : message(_message) {}
    const std::string& what() const { return message; }
};

#endif
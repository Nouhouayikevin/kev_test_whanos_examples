#pragma once
#include <queue>
#include <mutex>
#include <optional>
#include "Input.hpp"

// Thread-safe FIFO for inputs
class InputQueue {
private:
    std::queue<Input> q_;
    mutable std::mutex m_;
public:
    void push(const Input &in) {
        std::lock_guard<std::mutex> lock(m_);
        q_.push(in);
    }
    std::optional<Input> pop() {
        std::lock_guard<std::mutex> lock(m_);
        if (q_.empty()) return std::nullopt;
        Input v = q_.front();
        q_.pop();
        return v;
    }
    bool empty() const {
        std::lock_guard<std::mutex> lock(m_);
        return q_.empty();
    }
    size_t size() const {
        std::lock_guard<std::mutex> lock(m_);
        return q_.size();
    }
};

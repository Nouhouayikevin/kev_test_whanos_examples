#pragma once
#include <vector>
#include "Input.hpp"
#include "Snapshot.hpp"

class GameLogic {
public:
    void update(const std::vector<Input> &inputs);
    Snapshot getSnapshot() const;
};

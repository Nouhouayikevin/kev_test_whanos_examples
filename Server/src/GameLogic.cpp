#include "../include/GameLogic.hpp"

void GameLogic::update(const std::vector<Input> &inputs) {
    (void)inputs; // TODO: apply to world state
}

Snapshot GameLogic::getSnapshot() const {
    Snapshot s{};
    s.tick = 0; // update from a global or internal counter later
    return s;
}

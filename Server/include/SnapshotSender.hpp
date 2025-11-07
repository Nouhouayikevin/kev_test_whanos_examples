#pragma once
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include "Snapshot.hpp"
#include "Client.hpp"
#include "Protocol.hpp"

class SnapshotSender {
private:
    Protocol protocol_;
    boost::asio::ip::udp::socket& socket_;
    uint32_t roomId_;
    uint32_t seqCounter_;
    
public:
    SnapshotSender(boost::asio::ip::udp::socket& socket, uint32_t roomId);
    void sendToAll(const std::vector<Client> &clients, const Snapshot &snapshot);
};
 
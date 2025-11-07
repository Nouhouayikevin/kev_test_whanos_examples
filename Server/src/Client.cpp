#include "../include/Client.hpp"
#include <iostream>

Client::Client(unsigned int id, std::string addr)
{
    id_ = id;
    addr_ = addr;
}

void Client::setId(uint32_t id)
{
    id_ = id;
}

void Client::setAddr(std::string addr)
{
    addr_ = addr;
}

void Client::clear()
{
    id_ = 0;
    addr_ = "";
    lastSeqReceived = 0;
    lastSeenMs = 0;
}

bool Client::isEmpty(void)
{
    return id_ == 0 && lastSeqReceived == 0 && lastSeenMs == 0;
}

void Client::setRoomId(uint32_t roomId)
{
    room_id = roomId;
}

uint32_t Client::getRoomId(void) const
{
    return room_id;
}

uint32_t Client::getId(void) const
{
    return id_;
}

Client::~Client() {

    id_ = 0;
    addr_ = "";
    lastSeqReceived = 0;
    lastSeenMs = 0;
}
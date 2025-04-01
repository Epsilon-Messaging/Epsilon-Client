#pragma once
#include <string>

class Message {
public:
    int timestamp;
    std::string sender_name;
    std::string content;

    Message(int timestamp, std::string sender_name, std::string content)
        : timestamp(timestamp), sender_name(sender_name), content(content) {}
};


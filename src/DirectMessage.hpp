#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "User.hpp"
#include "Message.hpp"

class DirectMessage {
public:
    int id;
    User user1; // Always self
    User user2;
    std::vector<Message> messages;

    DirectMessage(int id, User user1, User user2, std::vector<Message> messages)
        : id(id), user1(user1), user2(user2), messages(messages) {}

    void addMessage(const Message& message) {
        messages.push_back(message);
    }

    void printDirectMessageDetails() {
        std::cout << "DirectMessage ID: " << id << std::endl;
        std::cout << "User 1: " << user1.getUsername() << std::endl;
        std::cout << "User 2: " << user2.getUsername() << std::endl;
        std::cout << "Messages: " << messages.size() << std::endl;
    }
};


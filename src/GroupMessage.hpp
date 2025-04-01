#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "User.hpp"
#include "Message.hpp"

class GroupMessage {
public:
    int id;
    int owner;
    std::string group_name;
    std::vector<User> users;
    std::vector<Message> messages;

    GroupMessage(int id, std::string group_name, int owner, 
                 std::vector<User> users, std::vector<Message> messages)
        : id(id), group_name(group_name), owner(owner), 
          users(users), messages(messages) {}

    void addMessage(const Message& message) {
        messages.push_back(message);
    }

    bool addUser(const User& user) {
        users.push_back(user);
        return true;
    }

    bool removeUser(int user_id) {
        return true;
    }

    void printGroupMessageDetails() const {
        std::cout << "GroupMessage ID: " << id << std::endl;
        std::cout << "Name: " << group_name << std::endl;
        std::cout << "Owner: " << (owner != -1 ? std::to_string(owner) : "None") << std::endl;
        std::cout << "Number of Users: " << users.size() << std::endl;
        std::cout << "Messages: " << messages.size() << std::endl;
    }
};


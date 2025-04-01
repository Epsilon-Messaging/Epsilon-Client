#pragma once
#include <vector>
#include "User.hpp"
#include "Message.hpp"
#include "EpsilonClient.hpp"

class Conversation {
  public:
  int owner; // Can remove people
  int id; // ID of conversation, fetch messages from DB using conversation_id
  std::vector<User> users;
  std::vector<Message> messages;

  Conversation() {
  
  }

  bool addUser() {
    // Fetch user
    User user = {"Peaches_MLG", "somepublickey"};
    users.push_back(user);
    return true;
  }

  bool removeUser() {
    return true;
  }

  bool sendMessage() {
    //for user : users {
    //  // etc...
    //}
    return true;
  }
};

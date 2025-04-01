#pragma once
#include <string>

class User {
public:
    std::string username;
    std::string public_key;
    int id;

    User(std::string username, std::string public_key, int id)
        : username(username), public_key(public_key), id(id) {}

    std::string getUsername() {
      return username;
    }
};


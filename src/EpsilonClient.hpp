#include <vector>
#include <string>
#include "Message.hpp"

class EpsilonClient {
  public:
    bool use_tor = false;

  private:
  static std::vector<Message> fetchMessages(std::string public_key, int count, int offset) {
    std::vector<Message> messages = {
      { 1743480424, "Tiqur", "Hello"},
      { 1743480429, "Peaches_MLG", "Hi"},
      { 1743480434, "Tiqur", "Check this out https://www.youtube.com/watch?v=dQw4w9WgXcQ"},
      { 1743480437, "Peaches_MLG", "Ok"},
      { 1743480443, "Peaches_MLG", ">:C"},
    };
    return messages;
  }

  static bool pushToWebSocket(std::string message_str) {
    return true;
  };

  static bool sendMessage(std::string public_key, Message message) {

    //std::string signed_message = EpsilonEncrypt.sign(message);
    //std::string encrypted_message = EpsilonEncrypt.encrypt(signed_message);
    //pushToWebSocket(encrypted_message)


    // Send, if success, append to local array. if not, discard and keep in input box
    bool success = true;

    if (success) {
      // Append to local messages
    } else {
      // Place message back in input box
    }

    return success;
  };
};

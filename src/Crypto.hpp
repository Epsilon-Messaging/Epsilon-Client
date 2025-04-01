#include <string>
#include "RNPWrapper.hpp"

class Crypto {
  public:
    std::string public_key;
    std::string private_key;
    rnp::RNPWrapper rnp;


  Crypto() {

  }

  int generate_keys() {

  }

  bool loadKeys() {
    // If no locally saved keys, create them
    if (true) {
      int result = rnp.ffi_generate_keys();

      // Save to disk
      //saveKeys(public_key, private_key);
    }
    return true;
  }

  bool saveKeys(std::string public_key, std::string private_key) {
    return true;
  }

  std::string signMessage(std::string message) {
    // Encrypt using own private key
    return "";
  }

  std::string encryptMessage(std::string message, std::string private_key) {
    // Encrypt using recipient's public key
    return "";
  }

  std::string decryptMessage(std::string message) {
    // Decrypt using own private key
    return "";
  }
    
};

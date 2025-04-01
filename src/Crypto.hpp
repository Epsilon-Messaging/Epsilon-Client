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
      int result_gen = rnp.ffi_generate_keys();
      int result_enc = rnp.ffi_encrypt();
      int result_dec1 = rnp.ffi_decrypt(true);
      int result_dec2 = rnp.ffi_decrypt(false);
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

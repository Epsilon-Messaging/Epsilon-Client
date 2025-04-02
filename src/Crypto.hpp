#include <string>
#include <iostream>
#include "RNPWrapper.hpp"

class Crypto {
public:
    std::string public_key;
    std::string private_key;
    rnp::RNPWrapper rnp;

    Crypto() {
    
    }

    int generate_keys() {
        return rnp.ffi_generate_keys();
    }

    bool loadKeys() {
        // If no locally saved keys, create them
        if (true) {
            int result_gen = rnp.ffi_generate_keys();
            std::cout << "Key generation result: " << result_gen << std::endl;
            
            // Test encryption
            const char* test_message = "Test encryption message";
            std::cout << "Original message: " << test_message << std::endl;
            
            std::string encrypted = rnp.ffi_encrypt_string(test_message, "rsa@key");
            std::cout << "Encrypted message length: " << encrypted.length() << std::endl;
            
            if (!encrypted.empty()) {
                // Test decryption with key
                std::string decrypted = rnp.ffi_decrypt_string(encrypted);
                std::cout << "Decryption result: " << decrypted << std::endl;
                
                if (decrypted == test_message) {
                    std::cout << "Key-based decryption successful" << std::endl;
                } else {
                    std::cout << "Key-based decryption failed" << std::endl;
                }
            } else {
                std::cout << "Encryption failed" << std::endl;
                return false;
            }
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

    std::string encryptMessage(std::string message, std::string recipient_public_key) {
        // Encrypt using recipient's public key
        std::cout << "Encrypting message for recipient: " << recipient_public_key << std::endl;
        std::string encrypted = rnp.ffi_encrypt_string(message.c_str(), recipient_public_key);
        std::cout << "Encryption completed, result length: " << encrypted.length() << std::endl;
        return encrypted;
    }

    std::string decryptMessage(std::string encrypted_message) {
        // Decrypt using own private key
        std::cout << "Decrypting message of length: " << encrypted_message.length() << std::endl;
        std::string decrypted = rnp.ffi_decrypt_string(encrypted_message);
        std::cout << "Decryption completed, result: " << decrypted << std::endl;
        return decrypted;
    }
};

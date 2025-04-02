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
            
            // Extract and log the public and private keys
            public_key = rnp.ffi_export_public_key("rsa@key");
            private_key = rnp.ffi_export_private_key("rsa@key");
            
            std::cout << "Public key:" << std::endl << public_key << std::endl;
            std::cout << "Private key:" << std::endl << private_key << std::endl;
            
            // Test encryption
            const char* test_message = "Test encryption message";
            std::cout << "Original message: " << test_message << std::endl;
            
            std::string encrypted = rnp.ffi_encrypt_string(test_message, "rsa@key");
            std::cout << "Encrypted message: " << std::endl << encrypted << std::endl;
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
            
            // Save the keys for future use
            saveKeys(public_key, private_key);
        }
        return true;
    }

    bool saveKeys(std::string public_key, std::string private_key) {
        // Store the keys (this is currently a stub)
        this->public_key = public_key;
        this->private_key = private_key;
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

#pragma once
#include <string.h>
#include <iostream>
#include <rnp/rnp.h>

#define RNP_SUCCESS 0

class RNPKeyHandleT {
  private:
  rnp_key_handle_t handle = nullptr;

  public:
  RNPKeyHandleT() {

  }

  void reset() {
    handle = nullptr;
    rnp_key_handle_destroy(handle);
  }

  rnp_key_handle_t* get_ref() {
    return &handle;
  }

  rnp_key_handle_t get() {
    return handle;
  }

  ~RNPKeyHandleT() {
    if (handle) {
      rnp_key_handle_destroy(handle);
    }
  }
};

class RNPOpEncryptT {
  private:
  rnp_op_encrypt_t op = nullptr;

  public:
  RNPOpEncryptT() {

  }

  rnp_op_encrypt_t* get_ref() {
    return &op;
  }

  rnp_op_encrypt_t get() {
    return op;
  }

  ~RNPOpEncryptT() {
    if (op) {
      rnp_op_encrypt_destroy(op);
    }
  }
};

class RNPFFIT {
  private:
  rnp_ffi_t ffi = nullptr;

  public:
  RNPFFIT() {

  }

  rnp_ffi_t* get_ref() {
    return &ffi;
  }

  rnp_ffi_t get() {
    return ffi;
  }

  ~RNPFFIT() {
    if (ffi) {
      rnp_ffi_destroy(ffi);
    }
  }
};

class RNPInputT {
  private:
  rnp_input_t input = nullptr;

  public: 
  RNPInputT() {}

  rnp_input_t get() {
    return input;
  }

  rnp_input_t* get_ref() {
    return &input;
  }

  void reset() {
    input = nullptr;
    rnp_buffer_destroy(input);
  }

  ~RNPInputT() {
    if (input) {
      rnp_buffer_destroy(input);
    }
  }
};

class RNPOutputT {
  private:
  rnp_output_t output = nullptr;

  public: 
  RNPOutputT() {}

  rnp_output_t get() {
    return output;
  }

  rnp_output_t* get_ref() {
    return &output;
  }

  void reset() {
    output = nullptr;
    rnp_buffer_destroy(output);
  }

  ~RNPOutputT() {
    if (output) {
      rnp_buffer_destroy(output);
    }
  }
};

class KeyGrip {
  private:
  char* kg = nullptr;

  public: 
  KeyGrip() {}

  char* get() {
    return kg;
  }

  char** get_ref() {
    return &kg;
  }

  void reset() {
    kg = nullptr;
    rnp_buffer_destroy(kg);
  }

  ~KeyGrip() {
    if (kg) {
      rnp_buffer_destroy(kg);
    }
  }
};

/* RSA key JSON description */
const char *RSA_KEY_DESC = "{\
    'primary': {\
        'type': 'RSA',\
        'length': 4096,\
        'userid': 'rsa@key',\
        'expiration': 0,\
        'usage': ['sign'],\
        'protection': {\
            'cipher': 'AES256',\
            'hash': 'SHA256'\
        }\
    },\
    'sub': {\
        'type': 'RSA',\
        'length': 4096,\
        'expiration': 0,\
        'usage': ['encrypt'],\
        'protection': {\
            'cipher': 'AES256',\
            'hash': 'SHA256'\
        }\
    }\
}";

namespace rnp {
  class RNPWrapper {
    static bool example_pass_provider(rnp_ffi_t        ffi,
                          void *           app_ctx,
                          rnp_key_handle_t key,
                          const char *     pgp_context,
                          char             buf[],
                          size_t           buf_len)
    {
        if (strcmp(pgp_context, "decrypt")) {
            strncpy(buf, "encpassword", buf_len);
            return true;
        }
        if (strcmp(pgp_context, "protect")) {
            return false;
        }
    
        return false;
    }

    public:
    static int ffi_generate_keys() {
        RNPOutputT keyfile;
        KeyGrip key_grip;
        RNPFFIT ffi;
        int result = 1;

        /* initialize RNPFFIT object */
        if (rnp_ffi_create(ffi.get_ref(), "GPG", "GPG") != RNP_SUCCESS) {
            std::cout << "Failed to create FFI" << std::endl;
            return result;
        }

        /* set password provider */
        if (rnp_ffi_set_pass_provider(ffi.get(), example_pass_provider, NULL)) {
            std::cout << "Failed to set password provider" << std::endl;
            return result;
        }

        /* generate RSA keypair */
        if (rnp_generate_key_json(ffi.get(), RSA_KEY_DESC, key_grip.get_ref()) != RNP_SUCCESS) {
            std::cout << "Failed to generate RSA key" << std::endl;
            return result;
        }

        std::cout << "Generated RSA key/subkey: " << key_grip.get() << std::endl;
        key_grip.reset();

        /* create file output object and save public keyring with generated keys, overwriting
         * previous file if any. You may use rnp_output_to_memory() here as well. */
        if (rnp_output_to_path(keyfile.get_ref(), "pubring.pgp") != RNP_SUCCESS) {
            std::cout << "Failed to initialize pubring.pgp writing" << std::endl;
            return result;
        }

        if (rnp_save_keys(ffi.get(), "GPG", keyfile.get(), RNP_LOAD_SAVE_PUBLIC_KEYS) != RNP_SUCCESS) {
            std::cout << "Failed to save pubring" << std::endl;
            return result;
        }

        keyfile.reset();

        /* create file output object and save secret keyring with generated keys */
        if (rnp_output_to_path(keyfile.get_ref(), "secring.pgp") != RNP_SUCCESS) {
            std::cout << "Failed to initialize secring.pgp writing" << std::endl;
            return result;
        }

        if (rnp_save_keys(ffi.get(), "GPG", keyfile.get(), RNP_LOAD_SAVE_SECRET_KEYS) != RNP_SUCCESS) {
            std::cout << "Failed to save secring" << std::endl;
            return result;
        }

        keyfile.reset();
        result = 0;
        std::cout << "Key generation completed successfully" << std::endl;
        return result;
    }

    static std::string ffi_encrypt_string(const char* message, const std::string& recipient_userid) {
        RNPFFIT ffi;
        RNPOpEncryptT encrypt;
        RNPKeyHandleT key;
        RNPInputT keyfile;
        RNPInputT input;
        RNPOutputT output;
        std::string result_str = "";
        
        std::cout << "Starting encryption of message for: " << recipient_userid << std::endl;
    
        if (rnp_ffi_create(ffi.get_ref(), "GPG", "GPG") != RNP_SUCCESS) {
            std::cout << "Failed to create FFI" << std::endl;
            return result_str;
        }
    
        if (rnp_input_from_path(keyfile.get_ref(), "pubring.pgp") != RNP_SUCCESS) {
            std::cout << "Failed to open pubring.pgp. Did you run key generation?" << std::endl;
            return result_str;
        }
    
        if (rnp_load_keys(ffi.get(), "GPG", keyfile.get(), RNP_LOAD_SAVE_PUBLIC_KEYS) != RNP_SUCCESS) {
            std::cout << "Failed to read pubring.pgp" << std::endl;
            return result_str;
        }
        keyfile.reset();
        std::cout << "Public keys loaded successfully" << std::endl;
    
        if (rnp_input_from_memory(input.get_ref(), (uint8_t *) message, strlen(message), false) != RNP_SUCCESS) {
            std::cout << "Failed to create input object" << std::endl;
            return result_str;
        }
    
        if (rnp_output_to_memory(output.get_ref(), 0) != RNP_SUCCESS) {
            std::cout << "Failed to create output object" << std::endl;
            return result_str;
        }
    
        if (rnp_op_encrypt_create(encrypt.get_ref(), ffi.get(), input.get(), output.get()) != RNP_SUCCESS) {
            std::cout << "Failed to create encrypt operation" << std::endl;
            return result_str;
        }
    
        rnp_op_encrypt_set_armor(encrypt.get(), true);
        rnp_op_encrypt_set_file_name(encrypt.get(), "message.txt");
        rnp_op_encrypt_set_file_mtime(encrypt.get(), (uint32_t) time(NULL));
        rnp_op_encrypt_set_cipher(encrypt.get(), RNP_ALGNAME_AES_256);
    
        // Use the provided recipient user ID to find the key
        if (rnp_locate_key(ffi.get(), "userid", recipient_userid.c_str(), key.get_ref()) != RNP_SUCCESS) {
            std::cout << "Failed to locate recipient key: " << recipient_userid << std::endl;
            return result_str;
        }
        std::cout << "Located key for encryption: " << recipient_userid << std::endl;
    
        if (rnp_op_encrypt_add_recipient(encrypt.get(), key.get()) != RNP_SUCCESS) {
            std::cout << "Failed to add recipient" << std::endl;
            return result_str;
        }
        key.reset();
    
        if (rnp_op_encrypt_execute(encrypt.get()) != RNP_SUCCESS) {
            std::cout << "Encryption failed" << std::endl;
            return result_str;
        }
        std::cout << "Encryption operation executed successfully" << std::endl;

        uint8_t* encrypted_buf = nullptr;
        size_t encrypted_len = 0;
        
        if (rnp_output_memory_get_buf(output.get(), &encrypted_buf, &encrypted_len, false) != RNP_SUCCESS) {
            std::cout << "Failed to get encrypted data" << std::endl;
            return result_str;
        }
        
        result_str = std::string(reinterpret_cast<char*>(encrypted_buf), encrypted_len);
        std::cout << "Encrypted data retrieved, length: " << encrypted_len << std::endl;
        rnp_buffer_destroy(encrypted_buf);
        
        return result_str;
    }

    static std::string ffi_decrypt_string(const std::string& encrypted_data) {
        RNPFFIT ffi;
        RNPInputT keyfile;
        RNPInputT input;
        RNPOutputT output;
        std::string result_str = "";
    
        std::cout << "Starting decryption of message" << std::endl;
    
        if (rnp_ffi_create(ffi.get_ref(), "GPG", "GPG") != RNP_SUCCESS) {
            std::cout << "Failed to create FFI" << std::endl;
            return result_str;
        }
    
        if (rnp_input_from_path(keyfile.get_ref(), "secring.pgp") != RNP_SUCCESS) {
            std::cout << "Failed to open secring.pgp. Did you run key generation?" << std::endl;
            return result_str;
        }

        if (rnp_load_keys(ffi.get(), "GPG", keyfile.get(), RNP_LOAD_SAVE_SECRET_KEYS) != RNP_SUCCESS) {
            std::cout << "Failed to read secring.pgp" << std::endl;
            return result_str;
        }
        keyfile.reset();
        std::cout << "Secret keys loaded successfully" << std::endl;
    
        rnp_ffi_set_pass_provider(ffi.get(), example_pass_provider, NULL);
        std::cout << "Password provider set up" << std::endl;
    
        if (rnp_input_from_memory(input.get_ref(), 
                                reinterpret_cast<const uint8_t*>(encrypted_data.data()), 
                                encrypted_data.size(), 
                                false) != RNP_SUCCESS) {
            std::cout << "Failed to create input object" << std::endl;
            return result_str;
        }
    
        if (rnp_output_to_memory(output.get_ref(), 0) != RNP_SUCCESS) {
            std::cout << "Failed to create output object" << std::endl;
            return result_str;
        }
    
        if (rnp_decrypt(ffi.get(), input.get(), output.get()) != RNP_SUCCESS) {
            std::cout << "Decryption failed" << std::endl;
            return result_str;
        }
        std::cout << "Decryption operation executed successfully" << std::endl;
    
        uint8_t* decrypted_buf = nullptr;
        size_t decrypted_len = 0;
        
        if (rnp_output_memory_get_buf(output.get(), &decrypted_buf, &decrypted_len, false) != RNP_SUCCESS) {
            std::cout << "Failed to get decrypted data" << std::endl;
            return result_str;
        }
        
        result_str = std::string(reinterpret_cast<char*>(decrypted_buf), decrypted_len);
        std::cout << "Decrypted data retrieved, length: " << decrypted_len << std::endl;
        rnp_buffer_destroy(decrypted_buf);
        
        return result_str;
    }
  };
};

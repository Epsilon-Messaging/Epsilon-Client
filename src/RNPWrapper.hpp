#pragma once
#include <string.h>
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
        'length': 2048,\
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
        'length': 2048,\
        'expiration': 0,\
        'usage': ['encrypt'],\
        'protection': {\
            'cipher': 'AES256',\
            'hash': 'SHA256'\
        }\
    }\
}";

const char *CURVE_25519_KEY_DESC = "{\
    'primary': {\
        'type': 'EDDSA',\
        'userid': '25519@key',\
        'expiration': 0,\
        'usage': ['sign'],\
        'protection': {\
            'cipher': 'AES256',\
            'hash': 'SHA256'\
        }\
    },\
    'sub': {\
        'type': 'ECDH',\
        'curve': 'Curve25519',\
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
        if (!strcmp(pgp_context, "decrypt (symmetric)")) {
            strncpy(buf, "encpassword", buf_len);
            return true;
        }
        if (!strcmp(pgp_context, "decrypt")) {
            strncpy(buf, "password", buf_len);
            return true;
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
            return result;
        }

        /* set password provider */
        if (rnp_ffi_set_pass_provider(ffi.get(), example_pass_provider, NULL)) {
            return result;
        }

        /* generate EDDSA/X25519 keypair */
        if (rnp_generate_key_json(ffi.get(), CURVE_25519_KEY_DESC, key_grip.get_ref()) != RNP_SUCCESS) {
            fprintf(stdout, "failed to generate eddsa key\n");
            return result;
        }

        fprintf(stdout, "Generated 25519 key/subkey:\n%s\n", key_grip.get());
        key_grip.reset();

        /* generate RSA keypair */
        if (rnp_generate_key_json(ffi.get(), RSA_KEY_DESC, key_grip.get_ref()) != RNP_SUCCESS) {
            fprintf(stdout, "failed to generate rsa key\n");
            return result;
        }

        fprintf(stdout, "Generated RSA key/subkey:\n%s\n", key_grip.get());
        key_grip.reset();

        /* create file output object and save public keyring with generated keys, overwriting
         * previous file if any. You may use rnp_output_to_memory() here as well. */
        if (rnp_output_to_path(keyfile.get_ref(), "pubring.pgp") != RNP_SUCCESS) {
            fprintf(stdout, "failed to initialize pubring.pgp writing\n");
            return result;
        }

        if (rnp_save_keys(ffi.get(), "GPG", keyfile.get(), RNP_LOAD_SAVE_PUBLIC_KEYS) != RNP_SUCCESS) {
            fprintf(stdout, "failed to save pubring\n");
            return result;
        }

        keyfile.reset();

        /* create file output object and save secret keyring with generated keys */
        if (rnp_output_to_path(keyfile.get_ref(), "secring.pgp") != RNP_SUCCESS) {
            fprintf(stdout, "failed to initialize secring.pgp writing\n");
            return result;
        }

        if (rnp_save_keys(ffi.get(), "GPG", keyfile.get(), RNP_LOAD_SAVE_SECRET_KEYS) != RNP_SUCCESS) {
            fprintf(stdout, "failed to save secring\n");
            return result;
        }

        keyfile.reset();
        result = 0;
        return result;
    }

    static int ffi_encrypt() {
        RNPFFIT ffi;
        RNPOpEncryptT encrypt;
        RNPKeyHandleT key;
        RNPInputT keyfile;
        RNPInputT input;
        RNPOutputT output;
        const char * message = "RNP encryption sample message";
        int result = 1;
    
        /* initialize RNPFFIT object */
        if (rnp_ffi_create(ffi.get_ref(), "GPG", "GPG") != RNP_SUCCESS) {
            return result;
        }
    
        /* load public keyring - we do not need secret for encryption */
        if (rnp_input_from_path(keyfile.get_ref(), "pubring.pgp") != RNP_SUCCESS) {
            fprintf(stdout, "failed to open pubring.pgp. Did you run ./generate sample?\n");
            return result;
        }
    
        /* we may use RNP_LOAD_SAVE_SECRET_KEYS | RNP_LOAD_SAVE_PUBLIC_KEYS as well */
        if (rnp_load_keys(ffi.get(), "GPG", keyfile.get(), RNP_LOAD_SAVE_PUBLIC_KEYS) != RNP_SUCCESS) {
            fprintf(stdout, "failed to read pubring.pgp\n");
            return result;
        }
        keyfile.reset();
    
        /* create memory input and file output objects for the message and encrypted message */
        if (rnp_input_from_memory(input.get_ref(), (uint8_t *) message, strlen(message), false) !=
            RNP_SUCCESS) {
            fprintf(stdout, "failed to create input object\n");
            return result;
        }
    
        if (rnp_output_to_path(output.get_ref(), "encrypted.asc") != RNP_SUCCESS) {
            fprintf(stdout, "failed to create output object\n");
            return result;
        }
    
        /* create encryption operation */
        if (rnp_op_encrypt_create(encrypt.get_ref(), ffi.get(), input.get(), output.get()) != RNP_SUCCESS) {
            fprintf(stdout, "failed to create encrypt operation\n");
            return result;
        }
    
        /* setup encryption parameters */
        rnp_op_encrypt_set_armor(encrypt.get(), true);
        rnp_op_encrypt_set_file_name(encrypt.get(), "message.txt");
        rnp_op_encrypt_set_file_mtime(encrypt.get(), (uint32_t) time(NULL));
        rnp_op_encrypt_set_compression(encrypt.get(), "ZIP", 6);
        rnp_op_encrypt_set_cipher(encrypt.get(), RNP_ALGNAME_AES_256);
        rnp_op_encrypt_set_aead(encrypt.get(), "None");
    
        /* locate recipient's key and add it to the operation context. While we search by userid
         * (which is easier), you can search by keyid, fingerprint or grip. */
        if (rnp_locate_key(ffi.get(), "userid", "rsa@key", key.get_ref()) != RNP_SUCCESS) {
            fprintf(stdout, "failed to locate recipient key rsa@key.\n");
            return result;
        }
    
        if (rnp_op_encrypt_add_recipient(encrypt.get(), key.get()) != RNP_SUCCESS) {
            fprintf(stdout, "failed to add recipient\n");
            return result;
        }
        key.reset();
    
        /* add encryption password as well */
        if (rnp_op_encrypt_add_password(
              encrypt.get(), "encpassword", RNP_ALGNAME_SHA256, 0, RNP_ALGNAME_AES_256) != RNP_SUCCESS) {
            fprintf(stdout, "failed to add encryption password\n");
            return result;
        }
    
        /* execute encryption operation */
        if (rnp_op_encrypt_execute(encrypt.get()) != RNP_SUCCESS) {
            fprintf(stdout, "encryption failed\n");
            return result;
        }
    
        fprintf(stdout, "Encryption succeeded. Encrypted message written to file encrypted.asc\n");
        result = 0;
        return result;
    }


    static int ffi_decrypt(bool usekeys) {
        RNPFFIT ffi;
        RNPInputT keyfile;
        RNPInputT input;
        RNPOutputT output;
        uint8_t* buf = NULL;
        size_t buf_len = 0;
        int result = 1;
    
        /* initialize FFI object */
        if (rnp_ffi_create(ffi.get_ref(), "GPG", "GPG") != RNP_SUCCESS) {
            return result;
        }
    
        /* check whether we want to use key or password for decryption */
        if (usekeys) {
            /* load secret keyring, as it is required for public-key decryption. However, you may
             * need to load public keyring as well to validate key's signatures. */
            if (rnp_input_from_path(keyfile.get_ref(), "secring.pgp") != RNP_SUCCESS) {
                fprintf(stdout, "failed to open secring.pgp. Did you run ./generate sample?\n");
                return result;
            }
    
            /* we may use RNP_LOAD_SAVE_SECRET_KEYS | RNP_LOAD_SAVE_PUBLIC_KEYS as well*/
            if (rnp_load_keys(ffi.get(), "GPG", keyfile.get(), RNP_LOAD_SAVE_SECRET_KEYS) != RNP_SUCCESS) {
                fprintf(stdout, "failed to read secring.pgp\n");
                return result;
            }
            keyfile.reset();
        }
    
        /* set the password provider */
        rnp_ffi_set_pass_provider(ffi.get(), example_pass_provider, NULL);
    
        /* create file input and memory output objects for the encrypted message and decrypted
         * message */
        if (rnp_input_from_path(input.get_ref(), "encrypted.asc") != RNP_SUCCESS) {
            fprintf(stdout, "failed to create input object\n");
            return result;
        }
    
        if (rnp_output_to_memory(output.get_ref(), 0) != RNP_SUCCESS) {
            fprintf(stdout, "failed to create output object\n");
            return result;
        }
    
        if (rnp_decrypt(ffi.get(), input.get(), output.get()) != RNP_SUCCESS) {
            fprintf(stdout, "public-key decryption failed\n");
            return result;
        }
    
        /* get the decrypted message from the output structure */
        if (rnp_output_memory_get_buf(output.get(), &buf, &buf_len, false) != RNP_SUCCESS) {
            return result;
        }
        fprintf(stdout,
                "Decrypted message (%s):\n%.*s\n",
                usekeys ? "with key" : "with password",
                (int) buf_len,
                buf);
    
        result = 0;
        return result;
    }

  };
};

#pragma once
#include <string.h>
#include <rnp/rnp.h>

#define RNP_SUCCESS 0


class FFI {
  private:
  rnp_ffi_t ffi = nullptr;

  public:
  FFI() {

  }

  rnp_ffi_t* get_ref() {
    return &ffi;
  }

  rnp_ffi_t get() {
    return ffi;
  }

  ~FFI() {
    rnp_ffi_destroy(ffi);
  }
};

class RNPOutput {
  private:
  rnp_output_t output = nullptr;

  public: 
  RNPOutput() {}

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

  ~RNPOutput() {
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
    static bool example_pass_provider(rnp_ffi_t ffi, void* app_ctx, rnp_key_handle_t key, const char* pgp_context, char buf[], size_t buf_len) {
        if (strcmp(pgp_context, "protect")) {
            return false;
        }
    
        strncpy(buf, "password", buf_len);
        return true;
    }

    public:
    static int ffi_generate_keys() {
        RNPOutput keyfile;
        KeyGrip key_grip;
        FFI ffi;
        int result = 1;

        /* initialize FFI object */
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
  };
};

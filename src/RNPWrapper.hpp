#pragma once
#include <string.h>
#include <rnp/rnp.h>

#define RNP_SUCCESS 0

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
        rnp_ffi_t ffi = NULL;
        rnp_output_t keyfile = NULL;
        char* key_grips = NULL;
        int result = 1;

        /* initialize FFI object */
        if (rnp_ffi_create(&ffi, "GPG", "GPG") != RNP_SUCCESS) {
            return result;
        }

        /* set password provider */
        if (rnp_ffi_set_pass_provider(ffi, example_pass_provider, NULL)) {
            goto finish;
        }

        /* generate EDDSA/X25519 keypair */
        if (rnp_generate_key_json(ffi, CURVE_25519_KEY_DESC, &key_grips) != RNP_SUCCESS) {
            fprintf(stdout, "failed to generate eddsa key\n");
            goto finish;
        }

        fprintf(stdout, "Generated 25519 key/subkey:\n%s\n", key_grips);
        /* destroying key_grips buffer is our obligation */
        rnp_buffer_destroy(key_grips);
        key_grips = NULL;

        /* generate RSA keypair */
        if (rnp_generate_key_json(ffi, RSA_KEY_DESC, &key_grips) != RNP_SUCCESS) {
            fprintf(stdout, "failed to generate rsa key\n");
            goto finish;
        }

        fprintf(stdout, "Generated RSA key/subkey:\n%s\n", key_grips);
        rnp_buffer_destroy(key_grips);
        key_grips = NULL;

        /* create file output object and save public keyring with generated keys, overwriting
         * previous file if any. You may use rnp_output_to_memory() here as well. */
        if (rnp_output_to_path(&keyfile, "pubring.pgp") != RNP_SUCCESS) {
            fprintf(stdout, "failed to initialize pubring.pgp writing\n");
            goto finish;
        }

        if (rnp_save_keys(ffi, "GPG", keyfile, RNP_LOAD_SAVE_PUBLIC_KEYS) != RNP_SUCCESS) {
            fprintf(stdout, "failed to save pubring\n");
            goto finish;
        }

        rnp_output_destroy(keyfile);
        keyfile = NULL;

        /* create file output object and save secret keyring with generated keys */
        if (rnp_output_to_path(&keyfile, "secring.pgp") != RNP_SUCCESS) {
            fprintf(stdout, "failed to initialize secring.pgp writing\n");
            goto finish;
        }

        if (rnp_save_keys(ffi, "GPG", keyfile, RNP_LOAD_SAVE_SECRET_KEYS) != RNP_SUCCESS) {
            fprintf(stdout, "failed to save secring\n");
            goto finish;
        }

        rnp_output_destroy(keyfile);
        keyfile = NULL;

        result = 0;
    finish:
        rnp_buffer_destroy(key_grips);
        rnp_output_destroy(keyfile);
        rnp_ffi_destroy(ffi);
        return result;
    }
  };
};

import gnupg
import os
import uuid

class GPGManager:
    def __init__(self, passphrase, gnupghome=None):
        if gnupghome:
            os.makedirs(gnupghome, exist_ok=True)
            self.gpg = gnupg.GPG(gnupghome=gnupghome)
        else:
            self.gpg = gnupg.GPG()
        self.passphrase = passphrase

    def generate_key(self):
        guid = str(uuid.uuid4())
        key_input = self.gpg.gen_key_input(
            name_real=guid,
            name_email=f"{guid}@guid.local",
            passphrase=self.passphrase,
            key_type="RSA",
            key_length=4096
        )
        key = self.gpg.gen_key(key_input)
        
        if not key:
            raise Exception("Failed to generate GPG key")
            
        print(f"Generated Key Fingerprint: {key.fingerprint}")
        return key.fingerprint
        
    def export_keys(self, fingerprint, output_prefix):
        public_key = self.gpg.export_keys(fingerprint)
        private_key = self.gpg.export_keys(fingerprint, secret=True, passphrase=self.passphrase)

        with open(f"{output_prefix}_public.asc", "w") as pub_file:
            pub_file.write(public_key)

        with open(f"{output_prefix}_private.asc", "w") as priv_file:
            priv_file.write(private_key)
            
        print(f"Keys exported to {output_prefix}_public.asc and {output_prefix}_private.asc")

    def sign_and_encrypt(self, message, sender_fingerprint, recipient_fingerprint):
        encrypted_data = self.gpg.encrypt(
            message,
            recipients=[recipient_fingerprint],
            sign=sender_fingerprint,
            passphrase=self.passphrase,
            always_trust=True
        )
        
        if not encrypted_data:
            raise Exception(f"Sign and encrypt failed: {encrypted_data.status}")
            
        return str(encrypted_data)


if __name__ == "__main__":
    gnupghome = "temp_gnupg"
    
    gpg_manager = GPGManager(passphrase="secure-passphrase", gnupghome=gnupghome)
    
    print("\nGenerating sender's key...")
    sender_fingerprint = gpg_manager.generate_key()
    gpg_manager.export_keys(sender_fingerprint, "sender")
    
    print("\nGenerating recipient's key...")
    recipient_fingerprint = gpg_manager.generate_key()
    gpg_manager.export_keys(recipient_fingerprint, "recipient")
    
    message = "Hello, this is a signed and encrypted message."

    print("\nSigning and encrypting message...")
    encrypted_message = gpg_manager.sign_and_encrypt(
        message,
        sender_fingerprint,
        recipient_fingerprint
    )
    
    print("\nEncrypted and signed message:\n", encrypted_message)

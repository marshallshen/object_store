#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <string.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

#define AES_BLOCK_SIZE 16
#define MAX_TEXT_SIZE 4096
void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int plaintext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}


int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext)
{
  EVP_CIPHER_CTX *ctx;

  int len;

  int ciphertext_len;

  /* Create and initialise the context */
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    handleErrors();

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

char *create_init_vector() {
  static int iv;
  static char civ[128];
  FILE *f = fopen("/dev/urandom", "r");

  fread(&iv, 1, 128, f); fclose(f);
  sprintf(civ, "%d", iv);

  printf("[Crypto Initialization Vector] IV is %s\n", civ);

  return civ;
}

unsigned char *md5_key(char *passphrase){
  unsigned char hex_key[MD5_DIGEST_LENGTH];
  static unsigned char key[256];

  MD5(passphrase, strlen(passphrase), hex_key);
  for(int i = 0; i < 256; i++)
    sprintf(&key[i], "%02x", (unsigned int)key[i]);

  return key;
}

typedef struct {
  int text_len;
  unsigned char *text;
} Crypto;

Crypto encrypted_text(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv){
  Crypto crypto;
  int ciphertext_len;
  static unsigned char *ciphertext;

  /* Encrypt the plaintext */
  ciphertext = malloc(strlen(plaintext));
  ciphertext_len = encrypt(plaintext, strlen(plaintext), key, iv,
    ciphertext);

  ciphertext[ciphertext_len] = '\0';
  crypto.text_len = ciphertext_len;
  crypto.text = malloc(ciphertext_len);
  strcpy(crypto.text, ciphertext);

  return crypto;
}

unsigned char *decrypted_text(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv){
  int plaintext_len;
  static unsigned char *plaintext;

  /* Decrypt the ciphertext */
  plaintext = malloc(ciphertext_len);
  plaintext_len = decrypt(ciphertext, ciphertext_len, key, iv, plaintext);

  /* Add a NULL terminator. We are expecting printable text */
  plaintext[plaintext_len] = '\0';

  return plaintext;
}

Crypto write_encrypted_file(char *filename, unsigned char *plaintext, unsigned char *key, unsigned char *iv) {
  FILE *f = fopen(filename, "wb");
  Crypto crypto = encrypted_text(plaintext, strlen(plaintext), key, iv);

  if (fwrite(crypto.text, 1, crypto.text_len, f) != crypto.text_len){
    fprintf(stderr, "write file error.\n");
    exit(1);
  };
  fclose(f);

  return crypto;
}

void read_encrypted_file(char *filename, int ciphertext_len, unsigned char *key, unsigned char *iv) {
  FILE *f = fopen(filename, "rb");
  unsigned char buf[ciphertext_len];

  while (feof(f) == 0) {
    if (fread(buf, 1, ciphertext_len, f) != ciphertext_len) {
      printf("Reading from file.. %s Decrypted text is:\n", filename);
      printf("%s\n", decrypted_text(buf, ciphertext_len, key, iv));
    }
  }
  fclose(f);
}

int main(int arc, char *argv[])
{
  char *passphrase = "passphrase";
  unsigned char *key = md5_key(passphrase);
  unsigned char *iv = create_init_vector();
  unsigned char *plaintext = "Today is a very good day, I will solve this problem soon - Marshall, Rachel.... And there are many many more words I want to say on how much you mean to me.";
  Crypto crypto;

  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  crypto = write_encrypted_file("test", plaintext, key, iv);
  read_encrypted_file("test", crypto.text_len, key, iv);

  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

  return 0;
}


#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <string.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

#define TEXT_BLOCK_SIZE 256
void handleErrors(char *message)
{
  printf("[Crypto SUPPORT ERROR] %s \n", message);
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
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors("DEC: fail to initialize new context.");

  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * We are using 128 bit AES (i.e. a 128 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors("DEC: fail to start decrypt init");

  /* Provide the message to be decrypted, and obtain the plaintext output.
   * EVP_DecryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors("DEC: fail to run decrypt update.");
  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
    printf("[CRYPTO SUPPORT ERROR WARNING]: Decryption error detected..\n\n");
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
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors("ENC: fail to initialize context.");

  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
   * and IV size appropriate for your cipher
   * We are using 128 bit AES (i.e. a 128 bit key). The
   * IV size for *most* modes is the same as the block size. For AES this
   * is 128 bits */
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    handleErrors("ENC: fail to initialize encryption.");

  /* Provide the message to be encrypted, and obtain the encrypted output.
   * EVP_EncryptUpdate can be called multiple times if necessary
   */
  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    handleErrors("ENC: fail to initiialize update.");
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors("ENC: fail to finalize encrypt.");
  ciphertext_len += len;

  /* Clean up */
  EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

#define DICTIONARY "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
char *create_init_vector() {
  static int iv;
  char *civ;
  int civ_len;
  FILE *f = fopen("/dev/urandom", "r");

  civ_len = 128 / sizeof(char *);
  civ = malloc(civ_len);

  for (int i = 0; i < civ_len; i++) {
    fread(&iv, sizeof(iv), 1, f);
    civ[i] = DICTIONARY[iv % strlen(DICTIONARY)];
  }
  fclose(f);

  return civ;
}

unsigned char *md5_key(char *passphrase){
  unsigned char hex_key[MD5_DIGEST_LENGTH];
  static unsigned char key[128];

  MD5(passphrase, strlen(passphrase), hex_key);
  for(int i = 0; i < 128; i++)
    sprintf(&key[i], "%02x", (unsigned int)key[i]);

  return key;
}

typedef struct {
  int text_len;
  unsigned char *text;
} Crypto;

Crypto encrypted_text(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *iv){
  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  Crypto crypto;
  int ciphertext_len;
  unsigned char *ciphertext;

  ciphertext = malloc(strlen(plaintext) * sizeof(char *));
  /* Encrypt the plaintext */
  ciphertext_len = encrypt(plaintext, strlen(plaintext), key, iv,
    ciphertext);

  ciphertext[ciphertext_len] = '\0';
  crypto.text_len = ciphertext_len;
  crypto.text = malloc(ciphertext_len);
  strcpy(crypto.text, ciphertext);

  /* Do something useful with the ciphertext here */
  printf("Ciphertext is:\n");
  BIO_dump_fp(stdout, ciphertext, ciphertext_len);

  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

  return crypto;
}

unsigned char *decrypted_text(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv){
  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  int plaintext_len;
  unsigned char *plaintext;

  plaintext = malloc(ciphertext_len);
  /* Decrypt the ciphertext */
  plaintext_len = decrypt(ciphertext, ciphertext_len, key, iv,
    plaintext);

  /* Add a NULL terminator. We are expecting printable text */
  plaintext[plaintext_len] = '\0';

  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

  return plaintext;
}

Crypto write_encrypted_file(char *filename, unsigned char *plaintext, unsigned char *key, unsigned char *iv) {
  Crypto crypto = encrypted_text(plaintext, strlen(plaintext), key, iv);

  FILE *f = fopen(filename, "wb");
  if (fwrite(crypto.text, 1, crypto.text_len, f) != crypto.text_len){
    fprintf(stderr, "write file error.\n");
    exit(1);
  };
  fclose(f);

  return crypto;
}

void read_encrypted_file(char *filename, int ciphertext_len, unsigned char *key, unsigned char *iv) {
  FILE *f = fopen(filename, "rb");
  unsigned char buf[ciphertext_len * sizeof(char *)];

  while (feof(f) == 0) {
    if (fread(buf, 1, ciphertext_len, f) != ciphertext_len) {
      printf("Reading from file %s. Decrypted text is:\n", filename);
      printf("%s\n", decrypted_text(buf, ciphertext_len, key, iv));
    }
    else if(feof(f) != 0);
  }
  fclose(f);
}


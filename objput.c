#include "acl_adapters.h"
#include "crypto_support.h"

#define BUFFERSIZE 128

void save_encryption_key(char *username, char *objname, int ciphertext_len, char *passphrase, unsigned char *iv) {
  char filename[strlen(username) + strlen(CONCAT) + strlen(objname) + 1];
  FILE *f;
  char ciphertext_size[20];

  sprintf(ciphertext_size, "%d", ciphertext_len);

  strcat(passphrase, DELIMITER);
  strcat(passphrase, iv);
  strcat(passphrase, DELIMITER);
  strcat(passphrase, ciphertext_size);
  snprintf(filename, sizeof(filename), "%s%s%s", username, CONCAT, objname);

  f = fopen(key_filename(filename), "w");
  fprintf(f, "%s", passphrase);
  fclose(f);

  printf("[Objput SUCCESS] Encryption file %s created. \n", key_filename(filename));
}

void create_encrypted_file(char *username, char *objname, char *passphrase){
  char filename[strlen(OBJECT_DIR) + strlen(username) + strlen(CONCAT) + strlen(objname) + 1];
  char buffer[BUFFERSIZE];
  char *plaintext;
  unsigned char *iv = create_init_vector();
  unsigned char *key = md5_key(passphrase, strlen(passphrase));

  snprintf(filename, sizeof(filename), "%s%s%s%s", OBJECT_DIR, username, CONCAT, objname);
  plaintext = calloc(1,1);

  // Get plaintext
  while (fgets(buffer, BUFFERSIZE, stdin) != NULL){
    plaintext = realloc(plaintext, strlen(plaintext) + 1 + strlen(buffer));
    if (!plaintext) { printf("[Objput ERROR]: fail to read input \n"); exit(-1); }
    strcat(plaintext, buffer);
  }

  // Write encrypted file
  Crypto crypto = write_encrypted_file(filename, plaintext, key, iv);
  // Save encryption key in a file
  save_encryption_key(username, objname, crypto.text_len, passphrase, iv);

  printf("[Objput SUCCESS] File %s created. \n", filename);
}

int main(int argc, char *argv[])
{
  char objname[MAX_DATA];
  char username[MAX_DATA];
  char passphrase[MAX_DATA];

  user_id = getuid();
  group_id = getgid();

  strcpy(objname, argv[argc-1]);
  strcpy(username, username_from_uid(user_id));
  strncpy(passphrase, find_optional_value(argc, argv, "-k"), MAX_DATA);

  create_acl(objname, user_id, group_id);
  create_encrypted_file(username, objname, passphrase);

  return 0;
}


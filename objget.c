#include "acl_adapters.h"
#include "crypto_support.h"

char *find_passphrase(char *objname) {
  char *key_line;
  char *token;
  key_line = malloc(MAX_DATA);

  FILE *f = fopen(key_filename(objname), READ);
  while (fgets(key_line, MAX_DATA, f) != NULL){
    token = strtok(key_line, DELIMITER);
  }
  fclose(f);

  return token;
}

unsigned char *find_iv(char *objname) {
  char *key_line;
  char *token;

  key_line = malloc(MAX_DATA);

  FILE *f = fopen(key_filename(objname), READ);
  while (fgets(key_line, MAX_DATA, f) != NULL){
    token = strtok(key_line, DELIMITER);
    token = strtok(NULL, DELIMITER);
  }
  fclose(f);
  return token;
}

char *find_ciphertext_len(char *objname){
  char *key_line;
  char *token;

  key_line = malloc(MAX_DATA);

  FILE *f = fopen(key_filename(objname), READ);
  while (fgets(key_line, MAX_DATA, f) != NULL){
    token = strtok(key_line, DELIMITER);
    token = strtok(NULL, DELIMITER);
    token = strtok(NULL, DELIMITER);
  }
  fclose(f);
  return token;

}

int main(int argc, char* argv[]) {
  char objname[MAX_DATA];
  char passphrase[MAX_DATA];
  unsigned char *key;
  int ciphertext_len;

  strcpy(objname, argv[argc-1]);
  strncpy(passphrase, find_optional_value(argc, argv, "-k"), MAX_DATA);

  if (strcmp(passphrase, find_passphrase(objname)) != 0) {
    printf("[OBJGET ERROR] Invalid passphrase");
    exit(-1);
  }

  if (file_exists(filename(objname))) {
    user_id = getuid();
    group_id = getgid();

    if (read_allowed(objname, user_id, group_id)) {
      key = md5_key(find_passphrase(objname), strlen(find_passphrase(objname)));
      ciphertext_len = atoi(find_ciphertext_len(objname));
      read_encrypted_file(filename(objname), ciphertext_len, key, find_iv(objname));
    }
    else { printf("[OBJGET PERMISSION DENIED] You can not read file %s. \n", filename(objname)); exit(0); }
  }
  else { printf("[OBJGET ERROR] File %s does not exist.\n", filename(objname)); exit(0); }

  return 0;
}


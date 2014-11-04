#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "constants.h"
#include "ugid_support.h"

char *filename(char *objname) {
  if (strstr(objname, OBJECT_DIR) != NULL)
    return objname;
  else{
    static char *filename;

    filename = malloc(MAX_DATA);
    snprintf(filename, MAX_DATA, "%s%s", OBJECT_DIR, objname);

    return filename;
  }
}

char *acl_filename(char *objname) {
  if (strstr(objname, ACL_DIR) != NULL)
    return objname;
  else {
    static char *acl_filename;

    acl_filename = malloc(MAX_DATA);
    snprintf(acl_filename, MAX_DATA, "%s%s%s", ACL_DIR, objname, ACL_EXT);

    return acl_filename;
  }
}

char *key_filename(char *objname) {
  if (strstr(objname, KEY_DIR) != NULL)
    return objname;
  else {
    static char *key_filename;

    key_filename = malloc(MAX_DATA);
    snprintf(key_filename, MAX_DATA, "%s%s%s", KEY_DIR, objname, KEY_EXT);

    return key_filename;
  }
}

char *find_optional_value(int argc, char *argv[], char *flag){
  static char *optional_value;

  optional_value = malloc(MAX_DATA);
  for (int i = 0; i< argc; i++) {
    if (strcmp(argv[i], flag) == 0)
      strncpy(optional_value, argv[i+1], MAX_DATA);
  }

  return optional_value;
}

int file_exists(char *filename){
  struct stat  buffer;
  return (stat (filename, &buffer) == 0);
}

int valid_access_string(char *access_string) {
  char *user_and_group_token, *access_token, *user_token, *group_token;
  int valid_user_and_group, valid_bits;

  user_and_group_token = strtok(access_string, SPACE);
  access_token = strtok(NULL, SPACE);

  user_token = strtok(user_and_group_token, DELIMITER);
  group_token = strtok(NULL, DELIMITER);

  valid_bits = 1;
  for (int i = 0; i < strlen(access_token)-1; i++){
    if (strchr(S_FULL, access_token[i]) == NULL) { valid_bits = 0; }
  }

  if (user_token != NULL && group_token != NULL)
    valid_user_and_group = 1;

  return ((valid_user_and_group == 1) && (valid_bits == 1));
}


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

char *permission_string(char *username, char *ownername,
                        char *groupname, char *full_access_details){
  char *permission_string = malloc(ACCESS_BITS);
  if(strcmp(username, ownername) == 0)
  {
    int user_start_bit = 0;
    for(int i = 0; i < ACCESS_BITS; i++){
      permission_string[i] = full_access_details[i+user_start_bit];
    }
  }
  else if(strcmp(username, groupname) == 0) {
    int group_start_bit = 5;
    for(int i = 0; i < ACCESS_BITS; i++){
      permission_string[i] = full_access_details[i+group_start_bit];
    }
  }
  else
  {
    int other_start_bit = 10;
    for(int i = 0; i < ACCESS_BITS; i++){
      permission_string[i] = full_access_details[i+other_start_bit];
    }
  }
  return permission_string;
}

char *get_permission(char *username, char *ownername,
                     char *groupname, char* filename){
  FILE *access_f = fopen(ACCESS_FILE, "r");
  char* line = NULL;
  size_t len = 0;
  ssize_t read;
  char access_string[MAX_DATA];

  //1. Get the right row from files_access
  while((read = getline(&line, &len, access_f)) != -1){
    if(strstr(line, filename) != NULL) {
      strcpy(access_string, line);
    }
  }

  //2. marshall+doc rwxpv -> rwxpv
  char full_access_details[MAX_DATA];
  char granted_access[ACCESS_BITS];

  char* token = strtok(access_string, " ");
  token = strtok(NULL, " ");
  strcpy(full_access_details, token);
  return permission_string(username, ownername, groupname, full_access_details);
}

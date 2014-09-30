#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "constants.h"
#include "extio.h"

void upsert_directory(char *username){
  struct stat dir_st = {0};
  char dir_name[strlen(OBJECT_DIR)  + strlen(username) + 1];
  snprintf(dir_name, sizeof(dir_name), "%s%s", OBJECT_DIR, username);
  if (stat(dir_name, &dir_st) == -1) {
    mkdir(dir_name, 0700);
  }
}

void set_owner_acl(char *username, char *objname){
  FILE *acl_f = fopen(ACCESS_FILE, "a+");
  // Default owner with ALL privilege
  fprintf(acl_f, "%s%s%s %s%s%s\n", username, "+", objname, DEFAULT_OWNER_PRIVILEGE, DEFAULT_GROUP_PRIVILEGE, DEFAULT_OTHER_PRIVILEGE);

  fclose(acl_f);
}

int main(int argc, char *argv[])
{
  char username[MAX_DATA];
  char groupname[MAX_DATA];
  char objname[MAX_DATA];
  char* slash = "/";

  strcpy(username, get_flagged_value(argc, argv, "-u"));
  strcpy(groupname, get_flagged_value(argc, argv, "-g"));
  strcpy(objname, argv[argc-1]);

  upsert_directory(username);

  //upsert file
  char filename[strlen(OBJECT_DIR) + strlen(username) + strlen(slash) + strlen(objname) + 1];
  snprintf(filename, sizeof(filename), "%s%s%s%s", OBJECT_DIR, username, slash, objname);

  //set acl
  struct stat file_st = {0};
  if (stat(filename, &file_st) == -1) {
    set_owner_acl(username, objname);
    //TODO set_group_acl(groupname, objname);
  }

  FILE *f = fopen(filename, "w");
  char text[MAX_DATA];
  while (fgets(text, MAX_DATA-1, stdin)){
   fprintf(f, "%s", text);
  }
  fclose(f);

  return 0;
}


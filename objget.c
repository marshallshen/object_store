#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "constants.h"
#include "extio.h"

int main(int argc, char* argv[])
{
  char username[MAX_DATA];
  char objname[MAX_DATA];
  char groupname[MAX_DATA];
  char* slash = "/";
  char* obj_dir = "objects";

  strcpy(username, get_flagged_value(argc, argv, "-u"));
  strcpy(groupname, get_flagged_value(argc, argv, "-g"));
  strcpy(objname, argv[argc-1]);
  // check dir exists
  struct stat st = {0};
  char dir_name[strlen(obj_dir) + strlen(slash) + strlen(username) + 1];
  snprintf(dir_name, sizeof(dir_name), "%s%s%s", obj_dir, slash, username);
  if (stat(dir_name, &st) == -1) {
    printf("directory %s does not exist", dir_name);
    exit(1);
  }

  // check if file exists, and print out file
  char filename[strlen(obj_dir) + strlen(slash) + strlen(username) + strlen(slash) + strlen(objname) + 1];
  snprintf(filename, sizeof(filename), "%s%s%s%s%s", obj_dir, slash, username, slash, objname);
  FILE *f = fopen(filename, "r");
  char c;

  if (f == NULL)
  {
    printf("can not find file %s", filename);
    exit(1);
  }

  while ((c = getc(f)) != EOF){
    putchar(c);
  }

  fclose(f);
  return 0;
}

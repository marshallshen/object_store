#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "constants.h"
#include "extio.h"

int main(int argc, char* argv[])
{
  char username[MAX_DATA];
  char* slash = "/";
  char* obj_dir = "objects";

  strcpy(username, get_flagged_value(argc, argv, "-u"));

  // check dir exists
  struct stat st = {0};
  char dir_name[strlen(obj_dir) + strlen(slash) + strlen(username) + 1];
  snprintf(dir_name, sizeof(dir_name), "%s%s%s", obj_dir, slash, username);
  if (stat(dir_name, &st) == -1) {
    printf("directory %s does not exist", dir_name);
    exit(1);
  }

  // loop through dir
  DIR *d = opendir(dir_name);
  if (d){
    while (1){
      struct dirent *entry;
      entry = readdir(d);
      if (!entry){
        break;
      }
      printf("%s\n", entry->d_name);
    }
    if (closedir(d)){
      printf("could not close %s", dir_name);
      exit(1);
    }
  }

  return 0;
}

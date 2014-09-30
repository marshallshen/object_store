#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "extio.h"
#include "permissions.h"

int main(int argc, char* argv[]){
  char username[MAX_DATA];
  char objname[MAX_DATA];
  char groupname[MAX_DATA];

  strcpy(username, get_flagged_value(argc, argv, "-u"));
  strcpy(groupname, get_flagged_value(argc, argv, "-g"));
  strcpy(objname, argv[argc-1]);

  //get filename and ownername for ACL lookup
  char filename[MAX_DATA];
  char ownername[MAX_DATA];
  if (strchr(objname, '+') == NULL)
    {
      snprintf(filename, sizeof(filename), "%s%s%s", username, "+", objname);
      strcpy(ownername, username);
    }
  else
    {
      snprintf(filename, sizeof(filename), "%s", objname);
      strcpy(ownername, strtok(objname, "+"));
    }

  char *permission = get_permission(username, ownername, groupname, filename);
  printf("%s \n", permission);

  return 0;
}

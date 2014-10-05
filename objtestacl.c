#include "acl_adapters.h"

int main(int argc, char *argv[]){
  char *action, *objname;

  user_id = getuid();
  group_id = getgid();

  action = malloc(1);
  strncpy(action, find_optional_value(argc, argv, "-x"), 1);

  objname = malloc(MAX_DATA);
  strncpy(objname, argv[argc-1], MAX_DATA);

  if (file_exists(filename(objname))){
    if (action_allowed(objname, user_id, group_id, action)) { printf("Allowed. \n"); }
    else { printf("Denied \n"); }
  }
  else { printf("[TEST ACL ERROR] File %s does not exist.\n", filename(objname));}

  return 0;
}

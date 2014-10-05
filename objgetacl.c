#include "acl_adapters.h"

int main(int argc, char* argv[]) {
  char objname[MAX_DATA];

  strcpy(objname, argv[argc-1]);
  user_id = getuid();
  group_id = getgid();

  if (view_allowed(objname, user_id, group_id)){
    if (file_exists(acl_filename(objname))) {
      printf("%s\n", find_acl(objname, user_id, group_id));
    }
    else { printf("[GET ACL ERROR] File %s does not exist.\n", acl_filename(objname)); exit(0); }
  }
  else { printf("[GET ACL PERMISSION DENIED] Permission to read ACL of file %s is deined.\n", filename(objname)); exit(0);}

  return 0;
}

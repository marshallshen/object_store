#include "acl_adapters.h"

int main(int argc, char *argv[]){
  char objname[MAX_DATA];
  const char *acls[MAX_ACL_ENTRIES];
  char text[MAX_DATA], cloned_text[MAX_DATA];
  int total_updated = 0;
  int valid_acl;

  strcpy(objname, argv[argc-1]);
  user_id = getuid();
  group_id = getgid();

  while(fgets(text, MAX_DATA-1, stdin)) {
    strncpy(cloned_text, text, MAX_DATA); // Hack to get around strtok() changes content on memory
    valid_acl = valid_access_string(cloned_text);
    if (valid_acl == 1 && total_updated <= MAX_ACL_ENTRIES) { acls[total_updated] = text; }
    else if (valid_acl == 1 && total_updated > MAX_ACL_ENTRIES) {
      printf("[SET ACL ERROR] Updating more than %i permissions is not allowed.\n", MAX_ACL_ENTRIES);
      exit(0);
    }
    else { printf("[SET ACL ERROR] Invalid acl input %s.\n", text); exit(-1); }
  }

  if (permit_allowed(objname, user_id, group_id)) {
    update_acl(objname, acls, total_updated,  user_id, group_id);
  }
  else { printf("[SET ACL PERMISSION DENIED] You are not allowed to modify permission of file %s. \n", filename(objname)); exit(0); }

  return 0;
}

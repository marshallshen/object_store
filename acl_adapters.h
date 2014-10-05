#include "acl_support.h"

static uid_t user_id;
static gid_t group_id;

/*
 * find_acl(): find an ACL entry for user or group, based on objname
 * Given user u1 has file f
 * And u1+f permission includes "u2.* rw"
 * Then find_acl(u1+f, u2, g2) should return "rw"
 */

char *find_acl(char *objname, uid_t user_id, gid_t group_id) {
  if (file_exists(acl_filename(objname))) {
    char *username, *group_name, *acl_line;

    username = username_from_uid(user_id);
    group_name = groupname_from_gid(group_id);

    acl_line = malloc(MAX_DATA);

    FILE *f = fopen(acl_filename(objname), READ);
    while (fgets(acl_line, MAX_DATA, f) != NULL){
      if (strstr(acl_line, username) != NULL) {
        char *token;
        token = strtok(acl_line, SPACE);
        return strtok(NULL, SPACE);
      }
      else if (group_name != NULL && strstr(acl_line, group_name) != NULL) {
        char *token;
        token = strtok(acl_line, SPACE);
        return strtok(NULL, SPACE);
      }
      else continue;
    }

  }
  else { printf("[FIND ACL ERROR] File %s does not exist. \n", acl_filename(objname)); exit(-1); }

  return EMPTY_STRING;
}

/*
 * create_acl(): create a new ACL of a file, with initial access string
 * Given user u, file f
 * Then we should have one entry "acls/u+f.acl"
 * And inside acls/u+f.acl, there's an initial entry "u.* rwxpv"
 * And there's an entry for admin group "*.admin pv"
*/

void create_acl(const char *objname, uid_t owner_id, gid_t group_id){
  char *owner_name, *group_name, *acl_file, *owner_acl_entry, *admin_acl_entry;
  int acl_file_size, owner_acl_entry_size, admin_acl_entry_size;

  owner_name = username_from_uid(owner_id);
  group_name = groupname_from_gid(group_id);

  acl_file_size = strlen(ACL_DIR) + strlen(owner_name) + strlen(CONCAT) + strlen(objname) + strlen(ACL_EXT) + 1;
  acl_file = malloc(acl_file_size);
  snprintf(acl_file, acl_file_size, "%s%s%s%s%s", ACL_DIR, owner_name, CONCAT, objname, ACL_EXT);

  if (file_exists(acl_file)) {
    printf("[CREATE ACL ERROR] Cannot override permission for %s+%s\n", owner_name, objname);
    exit(0);
  }
  else {
    owner_acl_entry_size = strlen(owner_name) + strlen(DELIMITER) + strlen(WILD_MATCH) + strlen(SPACE) + strlen(S_FULL) + 1;
    owner_acl_entry = malloc(owner_acl_entry_size);
    snprintf(owner_acl_entry, owner_acl_entry_size, "%s%s%s%s%s", owner_name, DELIMITER, WILD_MATCH, SPACE, S_FULL);

    admin_acl_entry_size = strlen(WILD_MATCH) + strlen(DELIMITER) + strlen(ADMIN) + strlen(SPACE) + strlen(PERMIT) + strlen(VIEW) + 1;
    admin_acl_entry = malloc(admin_acl_entry_size);
    snprintf(admin_acl_entry, admin_acl_entry_size, "%s%s%s%s%s%s", WILD_MATCH, DELIMITER, ADMIN, SPACE, PERMIT, VIEW);

    FILE *f = fopen(acl_file, "w");
    fprintf(f, "%s\n", owner_acl_entry);
    fprintf(f, "%s\n", admin_acl_entry);
    fclose(f);
  }
}

/*
 * update_acl(): update existing acl of an object using the new entry
 * Given user u has file f
 * And u+f has entry "rwxpv"
 * When admin sets acl of u+f to "rw"
 * Then system calls update_acl with (u+f, array of new acls, size of the array...)
 * And acl of u+f should be "rw"
 */

void update_acl(const char *objname, const char *acls[], const int size_of_acls,  uid_t user_id, gid_t group_id) {
  char *acl_file;
  int acl_file_size;

  acl_file_size = strlen(ACL_DIR) + strlen(objname) + strlen(ACL_EXT) + 1;
  acl_file = malloc(acl_file_size);
  snprintf(acl_file, acl_file_size, "%s%s%s", ACL_DIR, objname, ACL_EXT);

  if (file_exists(acl_file)) {
    if(remove(acl_file) == 0) {
      FILE *f = fopen(acl_file, "w");
      for (int i = 0; i <= size_of_acls; i++) { fprintf(f, "%s", acls[i]); }
      fclose(f);
    }
    else { printf("[UPDATE ACL ERROR] fail to update permissions of file %s", acl_file); exit(-1); }
  }
  else { printf("[UPDATE ACL ERROR] can not update permission of file %s because it does not exist. \n", acl_file); exit(0); }
}

/*
 * A series of ACL adapter helper methods that makes
 * it easy to check wheher certain action is allowed
 *
 * All the helper functions are based on find_acl()
 */

int action_allowed(char *objname,  uid_t user_id, uid_t group_id, char *action){
  return (strstr(find_acl(objname, user_id, group_id), action) != NULL);
}

int read_allowed(char *objname,  uid_t user_id, uid_t group_id){
  return action_allowed(objname, user_id, group_id, READ);
}

int write_allowed(char *objname,  uid_t user_id, uid_t group_id){
  return action_allowed(objname, user_id, group_id, WRITE);
}

int execute_allowed(char *objname,  uid_t user_id, uid_t group_id){
  return action_allowed(objname, user_id, group_id, EXECUTE);
}

int view_allowed(char *objname,  uid_t user_id, uid_t group_id){
  return action_allowed(objname, user_id, group_id, VIEW);
}

int permit_allowed(char *objname,  uid_t user_id, uid_t group_id){
  return action_allowed(objname, user_id, group_id, PERMIT);
}

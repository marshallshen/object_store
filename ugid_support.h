#include <pwd.h>
#include <grp.h>
#include <ctype.h>
#include "constants.h"

char *username_from_uid(uid_t uid) {
  struct passwd *pwd;
  pwd = getpwuid(uid);
  return (pwd == NULL) ? NULL : pwd->pw_name;
}

uid_t uid_from_username(const char *name) {
  struct passwd *pwd;
  pwd = getpwnam(name);
  return (pwd == NULL) ? -1 : pwd->pw_uid;
}

char *groupname_from_gid(gid_t gid){
  struct group *grp;
  grp = getgrgid(gid);
  return (grp == NULL) ? NULL : grp->gr_name;
}

gid_t gid_from_groupname(const char *name) {
  struct group *grp;
  grp = getgrnam(name);
  return (grp == NULL) ? -1 : grp->gr_gid;
}

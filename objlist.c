#include <dirent.h>
#include "acl_adapters.h"

int list_file_size(int argc, char* argv[]){
  for (int i = 0; i< argc; i++) {
   if (strcmp(argv[i], "-l") == 0) return 1;
  }
  return 0;
}

int main(int argc, char* argv[])
{
  char *username, *file_prefix;
  int display_file_size;
  DIR *d;
  struct dirent *entry;

  // Whether to display file size
  display_file_size = list_file_size(argc, argv);

  // Files are orgnanized as username+objname
  // First we need to know what username to use
  // as prefix
  username = malloc(MAX_DATA);
  username = find_optional_value(argc, argv, "-u");
  if (strlen(username) == 0) {
    user_id = getuid();
    file_prefix = malloc(MAX_DATA);
    file_prefix = username_from_uid(user_id);
  }
  else {
    user_id = uid_from_username(username);
    if (user_id == -1) { printf("[Objlist ERROR] User %s does not exist.\n", username); exit(-1); }

    file_prefix = malloc(MAX_DATA);
    file_prefix = username;
  }


  // Iterate through object dir, find the right match
  d = opendir(OBJECT_DIR);
  while(1) {
    entry = readdir(d);

    if (!entry) break;

    if (strstr(entry->d_name, file_prefix) != NULL) {
      if (view_allowed(entry->d_name, user_id, group_id)) {
        if (display_file_size == 1) {
          struct stat st;
          stat(entry->d_name, &st);
          printf("%s %lld \n", entry->d_name, st.st_size);
        }
        else { printf("%s\n", entry->d_name);}
      }
    }
  }

  if (closedir(d)) { printf("[OBJLIST ERROR] could not close %s", OBJECT_DIR); exit(-1); }

  return 0;
}

#include "acl_adapters.h"

int main(int argc, char* argv[]) {
  char objname[MAX_DATA];
  strcpy(objname, argv[argc-1]);

  if (file_exists(filename(objname))) {
    user_id = getuid();
    group_id = getgid();

    if (read_allowed(objname, user_id, group_id)) {
      FILE *f = fopen(filename(objname), READ);
      char c;

      while ((c = getc(f)) != EOF) { putchar(c); }
      fclose(f);
    }
    else { printf("[OBJGET PERMISSION DENIED] You can not read file %s. \n", filename(objname)); exit(0); }
  }
  else { printf("[OBJGET ERROR] File %s does not exist.\n", filename(objname)); exit(0); }

  return 0;
}

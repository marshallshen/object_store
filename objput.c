#include "acl_adapters.h"

void create_file(char *username, char *objname){
  char filename[strlen(OBJECT_DIR) + strlen(username) + strlen(CONCAT) + strlen(objname) + 1];
  char text[MAX_DATA];

  snprintf(filename, sizeof(filename), "%s%s%s%s", OBJECT_DIR, username, CONCAT, objname);

  FILE *f = fopen(filename, "w");
  while (fgets(text, MAX_DATA-1, stdin)){
   fprintf(f, "%s", text);
  }
  printf("File %s created. \n", filename);
  fclose(f);
}

int main(int argc, char *argv[])
{
  char objname[MAX_DATA];
  char username[MAX_DATA];

  user_id = getuid();
  group_id = getgid();

  strcpy(objname, argv[argc-1]);
  strcpy(username, username_from_uid(user_id));

  create_acl(objname, user_id, group_id);
  create_file(username, objname);

  return 0;
}


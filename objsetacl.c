#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "extio.h"
#include "permissions.h"

void set_permission(char *filename, char *updated_access){
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  // Create upated file access in .swp file
  FILE *access_f = fopen(ACCESS_FILE, "r");
  FILE *tmp_access_f = fopen(TMP_ACCESS_FILE, "w");
  while((read = getline(&line, &len, access_f)) != -1){
    if(strstr(line, filename) != NULL)
      {
        char* file_access = strtok(line, filename);
        file_access = strtok(NULL, " ");
        // Now only able to change owner's string
        for(int i = 0; i < ACCESS_BITS; i++){
          file_access[i] = updated_access[i];
        }
        fprintf(tmp_access_f, "%s %s", filename, file_access);
      }
    else
      {
        fprintf(tmp_access_f, "%s", line);
      }
  }
  fclose(access_f);
  fclose(tmp_access_f);

  // Swap ACCESS_FILE with ACCESS_FILE.swp
  if (remove(ACCESS_FILE) == 0)
    {
      char tmp_line[128];
      FILE *tmp_access_f = fopen(TMP_ACCESS_FILE, "r");
      FILE *new_access_f = fopen(ACCESS_FILE, "w");
      while(fgets(tmp_line, 128, tmp_access_f)){
        fprintf(new_access_f, "%s", tmp_line);
      }
    }
  else
    {
      printf("There was an error updating access file\n");
      exit(0);
    }

  if(remove(TMP_ACCESS_FILE) != 0){
    printf("Error occured when removing %s\n", TMP_ACCESS_FILE);
    exit(0);
  }
}

char *standardized_access_string(char *access_string){
  char *standardized_access = malloc(ACCESS_BITS);
  for (int i = 0; i < strlen(DEFAULT_FULL_PRIVILEGE); i++){
    if (access_string[i] ==  DEFAULT_FULL_PRIVILEGE[i])
      {
       standardized_access[i] = DEFAULT_FULL_PRIVILEGE[i];
      }
     else
     {
      standardized_access[i] = NOT_PERMITTED_CHAR[0];
     }
  }

  return standardized_access;
}

int main(int argc, char *argv[]){
  char username[MAX_DATA];
  char groupname[MAX_DATA];
  char objname[MAX_DATA];
  char access_file[MAX_DATA];
  char access_string[MAX_DATA];

  strcpy(username, get_flagged_value(argc, argv, "-u"));
  strcpy(groupname, get_flagged_value(argc, argv, "-g"));
  strcpy(objname, argv[argc-1]);

  FILE *f = fopen(access_file, "r");
  char text[MAX_DATA];
  while (fgets(text, MAX_DATA-1, stdin)){
    strcpy(access_string, text);
  }
  fclose(f);

  char user_and_group[MAX_DATA];
  char *new_access;

  //find new access
  char* token = strtok(access_string, " ");
  if(strstr(token, username) == NULL) {
    printf("access can only be changed for owner of the file");
    exit(1);
  }
  token = strtok(NULL, " ");

  //find username+objname
  char filename[MAX_DATA];
  if (strchr(objname, '+') == NULL)
    {
      snprintf(filename, sizeof(filename), "%s%s%s", username, "+", objname);
    }
  else
    {
      snprintf(filename, sizeof(filename), "%s", objname);
    }
  set_permission(filename, standardized_access_string(token));

  return 0;
}

#include <string.h>
#include "constants.h"

char *get_flagged_value(int argc, char *argv[], char *flag){
  char *flagged_value = malloc(MAX_DATA);

  for (int i = 0; i< argc; i++){
    int ret = strcmp(argv[i], flag);
    if (ret == 0){
      flagged_value = argv[i+1];
    }
   }
   return flagged_value;
}

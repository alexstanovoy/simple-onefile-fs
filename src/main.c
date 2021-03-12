//#include <file_system.h>
#include <stdio.h>
#include <string.h>

int main() {
  //char temp[2 * MAX_PATH_LENGTH];
  //gets(temp);
  //puts(temp);
  //printf("%d %d\n", VerifyPath(temp), strlen(temp));
  char s[256];
  strcpy(s, "/a/");
  char* token = strtok(s, "/");
  while (token) {
      printf("token: %s\n", token);
      fflush(stdout);
      token = strtok(NULL, "/");
  }
}

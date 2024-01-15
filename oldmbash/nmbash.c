#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#define MAXLI 2048

char cmd[MAXLI];
char path[MAXLI];
int pathidx;
void mbash();

int main(int argc, char** argv) {
  while (1) {
    printf("%s $ ",getenv("PWD"));
    fgets(cmd, MAXLI, stdin);
    mbash(cmd);
  }
  return 0;
}

void mbash() {
  printf("test : %s",cmd);
  
  char *arg[MAXLI] = {0};
  char *separ = strtok(cmd," ");
  int i = 0;
  while(separ != NULL){
    arg[i++] = separ;
    separ = strtok(NULL, " ");
  }
  
  if (strcmp(arg[0], "cd") == 0) {
    printf("cd success \n");
    printf("%scd ~",arg[1]);    
    
    if (strcmp(arg[1], "~") == 0) {
      printf("bien dedans \n");
      char *dir = getenv("HOME");
      chdir(dir);
    }
    
  }
}

/*
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {
    while (1) {
        char command[256];
        char *argv[32];
        int argc = 0;

        printf("$ ");
        scanf("%s", command);

        if (strcmp(command, "cd") == 0) {
            scanf("%s", command);
            chdir(command);
        } else if (strcmp(command, "pwd") == 0) {
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        } else if (strcmp(command, "ls") == 0) {
            argv[argc++] = command;
            char argument[256];
            while (scanf("%s", argument) == 1) {
                argv[argc++] = argument;
            }
            argv[argc] = NULL;
            char *envp[] = {NULL};
            execve(command, argv, envp);
        } else {
            argv[argc++] = command;
            char argument[256];
            while (scanf("%s", argument) == 1) {
                argv[argc++] = argument;
            }
            argv[argc] = NULL;
            char *envp[] = {NULL};
            execve(command, argv, envp);
        }
    }
    return 0;
}
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#define MAXLI 2048

//Pour compiler le fichier : gcc mbash.c -o mbash

char cmd[MAXLI];
char path[MAXLI];
int pathidx;
void mbash();

int main(int argc, char** argv) {
  while (1) {
    printf("%s $ ",getenv("PWD"));
    if(fgets(cmd, MAXLI, stdin) == NULL ){
      printf("\n");
      exit(0); 
    }
    if(strcmp(cmd, "\f\n") == 0){
      system("clear");
    }else{
      mbash(cmd);
      }
  }
  return 0;
}

void mbash() {
  cmd[strlen(cmd)-1]=0;
  char *args[MAXLI] = {0};
  char *separ = strtok(cmd," ");
  int i = 0;
  while(separ != NULL){
    args[i++] = separ;
    separ = strtok(NULL, " ");
  }

  if(strcmp(args[0],"pwd") == 0){
    if(args[1] == NULL)
      printf("%s\n",getenv("PWD"));
  }
  else if(strcmp(args[0], "exit") == 0){
    exit(0);
  }
  else if (strcmp(args[0], "cd") == 0) {
    
    if (strcmp(args[1], "~") == 0) {
        char *dir = getenv("HOME");
        chdir(dir);
        setenv("PWD", dir, 1);
    }else if(chdir(args[1]) == 0){
      char cwd[256];
      char *dir = getcwd(cwd, sizeof(cwd));
      setenv("PWD",dir,1);
    }
    
  }
  else{
  	system(cmd);
  }
}


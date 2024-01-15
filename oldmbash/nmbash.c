#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAXLI 2048

char cmd[MAXLI];

void mbash();

int main(int argc, char** argv) {
    while (1) {
        // Utilisez la variable d'environnement PWD pour afficher le répertoire courant
        printf("%s $ ", getenv("PWD"));
        fgets(cmd, MAXLI, stdin);

        // Supprimer le caractère de nouvelle ligne à la fin de la commande
        size_t len = strlen(cmd);
        if (len > 0 && cmd[len - 1] == '\n') {
            cmd[len - 1] = '\0';
        }

        mbash(cmd);
    }
    return 0;
}

void mbash(char* command) {
    printf("Commande: %s\n", command);

    char* args[MAXLI] = {0};
    char* token = strtok(command, " ");
    int i = 0;

    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }

    if (i > 0) {
        if (strcmp(args[0], "cd") == 0) {
            if (i == 2) {
                // Si la commande est "cd" avec un argument
                printf("Changement de répertoire vers %s\n", args[1]);

                if (strcmp(args[1], "~") == 0) {
                    // Si l'argument est "~", changez vers le répertoire home
                    char* home_dir = getenv("HOME");
                    chdir(home_dir);
                } else {
                    // Sinon, changez vers le répertoire spécifié
                    chdir(args[1]);
                }
            } else {
                // Gestion d'erreur si la commande "cd" n'a pas le bon nombre d'arguments
                fprintf(stderr, "Usage: cd <directory>\n");
            }
        } else if (strcmp(args[0], "ls") == 0) {
            // Si la commande est "ls", exécutez-la directement
            pid_t pid = fork();

            if (pid == -1) {
                perror("Erreur lors de la création du processus fils");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) { // Processus fils
                execvp(args[0], args);
                perror("mbash");
                exit(EXIT_FAILURE);
            } else { // Processus parent
                int status;
                waitpid(pid, &status, 0);
                // Vous pouvez ajouter du code ici pour gérer le statut du processus fils si nécessaire
            }
        } else {
            // Pour les autres commandes, utilisez fork et execvp comme auparavant
            pid_t pid = fork();

            if (pid == -1) {
                perror("Erreur lors de la création du processus fils");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) { // Processus fils
                execvp(args[0], args);
                perror("mbash");
                exit(EXIT_FAILURE);
            } else { // Processus parent
                int status;
                waitpid(pid, &status, 0);
                // Vous pouvez ajouter du code ici pour gérer le statut du processus fils si nécessaire
            }
        }
    }
}




/*
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

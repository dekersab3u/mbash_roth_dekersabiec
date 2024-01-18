#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#define MAXLI 2048

//commande pour installer la librairie readline : sudo apt-get install libreadline-dev
//commande pour compiler le programme :  gcc -o mbash mbash.c -lreadline

void executeCommand(char* command);
void changeDirectory(char* directory);

void printUserHost() {
    char* username = getlogin();
    char hostname[MAXLI];

    if (gethostname(hostname, sizeof(hostname)) != -1) {
        printf("\033[1;36m%s@%s\033[0m", username, hostname);
    } else {
        perror("Erreur lors de la récupération du nom de l'hôte");
    }
}

void printRedPWD() {
    char cwd[MAXLI];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\033[1;31m%s\033[0m", cwd);
    } else {
        perror("Erreur lors de la récupération du répertoire courant");
    }
}

int main(int argc, char** argv) {
    while (1) {
        printUserHost();
        printf(" ");
        printRedPWD();
        printf(" $");

        char* input = readline(" ");
        

        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        executeCommand(input);
        add_history(input);

        free(input);
    }

    return 0;
}

void executeCommand(char* command) {
    printf("Commande: %s\n", command);
    
    if (strcmp(command, "exit") == 0) {
        printf("Fermeture du terminal.\n");
        exit(0);
    }

    char* args[MAXLI] = {0};
    char* token = strtok(command, " ");
    int i = 0;
    int background = 0;

    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            background = 1;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " ");
    }

    if (i > 0) {
        if (strcmp(args[0], "cd") == 0) {
            if (i == 2) {
                changeDirectory(args[1]);
            } else {
                //fprintf(stderr, "Usage: cd <directory>\n");
                char* home_dir = getenv("HOME");
        	chdir(home_dir);
        	printf("Changement du répertoire vers ~\n");
            }
        } else {
            pid_t pid = fork();

            if (pid == -1) {
                perror("Erreur lors de la création du processus fils");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) {
                if (background) {
                    int null_fd = open("/dev/null", O_WRONLY);
                    dup2(null_fd, STDOUT_FILENO);
                    close(null_fd);
                }

                execvp(args[0], args);
                perror("mbash");
                exit(EXIT_FAILURE);
            } else {
                if (!background) {
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
        }
    }
}

void changeDirectory(char* directory) {
    printf("Changement de répertoire vers %s\n", directory);

    if (strcmp(directory, "~") == 0) {
        char* home_dir = getenv("HOME");
        chdir(home_dir);
    } else {
        if (access(directory, F_OK) != -1) {
            chdir(directory);
        } else {
            fprintf(stderr, "Le répertoire %s n'existe pas\n", directory);
        }
    }
}

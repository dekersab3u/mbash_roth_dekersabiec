#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

#define MAXLI 2048

char cmd[MAXLI];
char path[MAXLI];

void mbash();

int main(int argc, char** argv) {
    while (1) {
        printf("Commande : ");
        fgets(cmd, MAXLI, stdin);
        mbash();
    }
    return 0;
}

void mbash() {
    // Supprimer le caractère de nouvelle ligne de l'entrée
    size_t len = strlen(cmd);
    if (len > 0 && cmd[len - 1] == '\n') {
        cmd[len - 1] = '\0';
    }

    // Tokeniser la commande d'entrée
    char* token = strtok(cmd, " ");

    if (token == NULL) {
        // Commande vide, ne rien faire
        return;
    }

    if (strcmp(token, "cd") == 0) {
        // Changer de répertoire
        token = strtok(NULL, " ");
        if (token != NULL) {
            if (chdir(token) == -1) {
                perror("cd");
            }
        } else {
            fprintf(stderr, "cd : argument manquant\n");
        }
    } else if (strcmp(token, "pwd") == 0) {
        // Afficher le répertoire de travail actuel
        if (getcwd(path, sizeof(path)) != NULL) {
            printf("%s\n", path);
        } else {
            perror("pwd");
        }
    } else {
        // Exécuter la commande
        int en_arriere_plan = 0;

        // Vérifier si la commande doit être exécutée en arrière-plan
        char* dernier_token = token;
        while (token != NULL) {
            dernier_token = token;
            token = strtok(NULL, " ");
        }

        if (dernier_token != NULL && strcmp(dernier_token, "&") == 0) {
            en_arriere_plan = 1;
        }

        // Créer un nouveau processus pour l'exécution de la commande
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
        } else if (pid == 0) {
            // Processus enfant
            if (en_arriere_plan) {
                // Rediriger l'entrée, la sortie standard et l'erreur vers /dev/null pour les processus en arrière-plan
                freopen("/dev/null", "r", stdin);
                freopen("/dev/null", "w", stdout);
                freopen("/dev/null", "w", stderr);
            }

            // Exécuter la commande
            if (execvp(cmd, (char* const*)NULL) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Processus parent
            if (!en_arriere_plan) {
                // Attendre que le processus enfant se termine s'il n'est pas en arrière-plan
                int statut;
                waitpid(pid, &statut, 0);
            }
        }
    }
}


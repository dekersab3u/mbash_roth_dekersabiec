#include <stdio.h>
#include <unistd.h>
#include <string.h>

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

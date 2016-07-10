/*  
 *  Dependencies
 *
#include <sys/wait.h>
    waitpid() and associated macros
#include <unistd.h>
    chdir()
    fork()
    exec()
    pid_t
#include <stdlib.h>
    malloc()
    realloc()
    free()
    exit()
    execvp()
    EXIT_SUCCESS, EXIT_FAILURE
#include <stdio.h>
    fprintf()
    printf()
    stderr
    getchar()
    perror()
#include <string.h>
    strcmp()
    strtok()
*/
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void shell_loop(void);
char *shell_readLine(void);
char **shell_split_line(char *line);
int shell_launch(char **args);
int shell_execute(char **args);


/*
 *  builtin commands
 */
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);
int shell_happy(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "happy"
};

int (*builtin_func[]) (char **) = {
    &shell_cd,
    &shell_help,
    &shell_exit,
    &shell_happy
};

int shell_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int shell_cd(char **args) {
    if(args[1] == NULL) {
        fprintf(stderr, "shell: expected arg to \"cd\"\n");
    } else {
        if(chdir(args[1]) != 0) {
            perror("shell");
        }
    }
    return 1;
}

int shell_help(char **args) {
    int i;
    printf("\nThis is a shell.\n\n");
    printf("The following commands are built in:\n");
    for(i=0;i<shell_num_builtins();i++) {
        printf("    %s\n",builtin_str[i]);
    }
    printf("\na man is a dogs best friend\n");
    printf("...good day\n\n");
    return 1;
}

int shell_exit(char **args) {
    return 0;
}

int happy(char **args) {
    if(args[1] == NULL) {
        printf("Don't worry...i got this...");
    } else {
        switch(args[1]) {
            case "sad" : printf("\n\nCheer up its only a game...\n(╯︵╰,)\n\n");
                         break;
            default    : printf("\n\nSure, why not?...\nε(๏_๏)з\n\n");
                         break;
        }

    }
    return 1;
}
















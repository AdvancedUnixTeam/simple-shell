#define SHELL_RL_BUFFSIZE 1024
#define SHELL_TOK_BUFFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"

#include "shell.h"

int main(int argc, char **argv)
{
	// Config Files
	// Command Line Loop
	shell_loop();
	// Shut Down / Clean Up
	return EXIT_SUCCESS;
}

void shell_loop(void)
{
	char *line;
	char **args;
	int status;
	do
	{
		printf("【ツ】 ");
		line = shell_readLine();
		args = shell_split_line(line);
		status = shell_execute(args);
		free(line);
		free(args);
	} while (status);
}

/*    
 *  The code in this method could be replaced with the following:
 *      
 *      char *line = NULL;
 *      ssize_t bufsize = 0; // have getline allocate a buffer for us
 *      getline(&line, &bufsize, stdin);
 *      return line;
 *  
 *  Keeping it this way for more control of buffer reallocation later on
 */
char *shell_readLine(void)
{
	int buffsize = SHELL_RL_BUFFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * buffsize);
	int c;
    if(!buffer) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);
    }
    while(1) {
        // Read a character
        c = getchar();
        // If EOF, replace with null and return
        if(c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;
        // Check if we have exceeded the buffer and reallocate if necessary
        if(position >= buffsize) {
            buffsize += SHELL_RL_BUFFSIZE;
            buffer = realloc(buffer, buffsize);
            if(!buffer) {
                fprintf(stderr, "shell: allocation error\n");
                exit(EXIT_FAILURE); 
            }
        }
    }
}

/* 
 *  Does not allow quoting or backslash escaping in command line args
 */
char **shell_split_line(char *line) {
    int buffsize = SHELL_TOK_BUFFSIZE, position = 0;
    char **tokens = malloc(buffsize * sizeof(char*));
    char *token;
    if(!tokens) {
        fprintf(stderr, "shell: allocation error\n");
        exit(EXIT_FAILURE);        
    }
    token = strtok(line, SHELL_TOK_DELIM);
    while(token != NULL) {
        tokens[position] = token;
        position++;
        if(position >= buffsize) {
            buffsize += SHELL_TOK_BUFFSIZE;
            tokens = realloc(tokens, buffsize * sizeof(char*));
            if(!tokens) {
                fprintf(stderr, "shell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, SHELL_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int shell_launch(char **args) {
    pid_t pid, wpid;
    int status;
    pid=fork();
    if(pid==0) {
        if(execvp(args[0],args)==-1) {
            perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0) {
        perror("shell");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }    
    return 1;
}

int shell_execute(char **args) {
    int i;
    if(args[0] == NULL) {
        return 1;
    }
    for(i=0; i<shell_num_builtins(); i++) {
        if(strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i]) (args);
        }
    }
    return shell_launch(args);
}

















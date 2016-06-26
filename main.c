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
		printf("{▬▬ι═══════ﺤ }");
		line = shell_readLine();
		args = shell_splitLine(line);
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
#define SHELL_RL_BUFFSIZE 1024
char *shell_readLine(void)
{
	int buffsize = SHELL_BUFFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * buffsize);
	int c;
    if(!buffer) {
        fprintf(stderr, "shell: allocation error\n")
        exit(EXIT_FAULURE);;
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
#define SHELL_TOK_BUFFSIZE 64
#define SHELL_TOK_DELIM " \t\r\n\a"
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




















int main(int argc, char **argv)
{
	// Config Files

	// Command Line Loop
	cmd_loop();
	
	// Shut Down / Clean Up
	
	return EXIT_SUCCESS;
}

void cmd_loop(void)
{
	char *line;
	char **args;
	int status;
	
	do
	{
		printf("{▬▬ι═══════ﺤ }");
		line = cmd_readLine();
		args = cmd_splitLine(line);
		status = cmd_execute(args);

		free(line);
		free(args);
	} while (status);
}

#define CMD_BUFFSIZE 1024
char *cmd_readLine(void)
{
	int buffsize = CMD_BUFFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * buffsize);
	int c;
}

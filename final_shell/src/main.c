#include <regex.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <commands.h>

/*************************************************
 * Description:
 *	This is a built-in function.
 *	It takes care of wildcards in arguments
 *	by compiling them as regular expressions.
 * In: 
 *	arg - argument containing wildcards
 * Out:	 
 * 
 * Author:
 *	Gustavo Rodriguez-Rivera and Justin Ennen 
 *	Introduction to Systems Programming: 
 *		a Hands-on Approach (V2015-2-25)
 *************************************************/
void expandWildcardsIfNecessary(char * arg)
{
        //Return if arg does not contain "*" or "?"
        if (arg has neither "*" nor "?" (use strchr) ) {
                Command._currentSimpleCommand->insertArgument(arg);
                return;
        }

        // 1. Convert wildcard to regular expression
        // Convert "*" -> ".*"
        // "?" -> "."
        // "." -> "\." and others you need
        // Also add ^ at the beginning and $ at the end to match
        // the beginning and the end of the word.
        // Allocate enough space for regular expression
        char * reg = (char*)malloc(2*strlen(arg)+10);
        char * a = arg;
        char * r = reg;
        *r = '^'; r++; //match beginning of line
        while (*a) {
                if (*a == '*') { *r='.'; r++; *r='*'; r++; }
                else if (*a == '?') { *r='.' r++; }
                else if (*a == '.') { *r='\\'; r++; *r='.'; r++;}
                else { *r=*a; r++;}
                a++;
        }
        *r='$'; r++; *r=0; //match end of line and add null char
        // 2. compile regular expression. See lab3-src/regular.cc
        char * expbuf = regcomp( reg, REG_EXTENDED|REG_NOSUB );
        if (expbuf==NULL) {
                perror("regcomp");
                return;
        }
        // 3. List directory and add as arguments the entries
        // that match the regular expression
        DIR * dir = opendir(".");
        if (dir == NULL) {
                perror("oopendir");
                return;
        }
        struct dirent * ent;
        while ( (ent = readdir(dir)) != NULL) {
                // Check if name matches
                if (regexec(ent->d_name, re ) == 0 ) {
                //Add argument
                Command._currentSimpleCommand->insertArgument(strdup(ent->d_name));
                }
        }
        closedir(dir);
}

/*************************************************
 * Description:
 *      Executes commands based on command table.
 * In: 
 *    
 * Out:  
 * 
 * Author:
 *      Gustavo Rodriguez-Rivera and Justin Ennen
 *      Introduction to Systems Programming:
 *              a Hands-on Approach (V2015-2-25)
 *************************************************/
void execute_command() {
	// save in/out
	int tmpin=dup(0);
	int tmpout=dup(1);

	// set the initial input
	int fdin;
	// infile not defined
	if (infile) 
		fdin = open(infile,O_READ);
	else // Use default input
		fdin=dup(tmpin);

	int ret;
	int fdout;
	for (i=0;i<numsimplecommands; i++) {
		// redirect input
		dup2(fdin, 0);
		close(fdin);
		//setup output
		if (i == numsimplecommands-1) {
			// Last simple command
			// outfile not defined
			if(outfile)
				fdout=open(outfile,â€|â€|);
			else // Use default output
				fdout=dup(tmpout);
		}
		else {
			// Not last
			// simple commmand
			// create pipe
			int fdpipe[2];
			pipe(fdpipe);
			pdout=fdpipe[1];
			fdin=fdpipe[0];
		}// if/else

		// Redirect output
		dup2(fdout,1);
		close(fdout);

		// Create child process
		ret=fork();
		if(ret==0) {
			// scmd not defined
			execvp(scmd[i].args[0], scmd[i].args);
			perrror(â€œexecvpâ€);
			_exit(1);
		}
	} // for

	// restore in/out defaults
	dup2(tmpin,0);
	dup2(tmpout,1);
	close(tmpin);
	close(tmpout);

	if (!background) // Wait for last command
		waitpid(ret, NULL);
} // execute


